//
// Created by healy on 11/10/2023.
//

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include "FindFiles.h"

#include  <stdio.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <stdlib.h>
#include  <sys/stat.h>
#include  <string.h>
#include <regex.h>

bool matchesRegex(char *regexString, char *input) {
    if (regexString == NULL) return false;
    regex_t regex;
    regcomp(&regex, regexString, REG_EXTENDED);
    int match = regexec(&regex, input, 0, NULL, 0);
    return match == 0;
}

// 777 in octal
int listIdx = 0;
#define PERMISSIONS 511

//Also IDK why this is warning me about recursion; I guess that's illegal?
void listAllFilesInternal(Config *c, char *path, MySyncFile **list) {
    char pathFromDirectory[MAX_PATH_LENGTH];
    int len = 0;
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c->directories[i] == NULL) break;
        sprintf(pathFromDirectory, "%s%s", c->directories[i], path);
        DIR *folder;
        struct dirent *dp;
        folder = opendir(pathFromDirectory);
        if (folder == NULL) {
            perror("Error");
            continue;
        }
        while ((dp = readdir(folder)) != NULL) {
            if (dp->d_name[0] == '.' && !c->allMode) continue;
            struct stat stats;
            char fullPath[MAX_PATH_LENGTH * 2];

            strcpy(fullPath, pathFromDirectory);
            strcat(fullPath, "/");
            strcat(fullPath, dp->d_name);
            if (stat(fullPath, &stats) != 0) {
                perror("Error");
            } else if (S_ISDIR(stats.st_mode)) {
                if (c->recursive && dp->d_name[0] != '.') {
                    if (c->verboseMode) {
                        printf("entering folder %s\n", fullPath);
                    }
                    char prefix2[MAX_PATH_LENGTH];
                    strcpy(prefix2, path);
                    strcat(prefix2, "/");
                    strcat(prefix2, dp->d_name);
                    listAllFilesInternal(c, prefix2, list);
                }
            } else if (S_ISREG(stats.st_mode)) {
                if (c->verboseMode) {
                    printf("%s is a regular file that was edited at %d\n", fullPath, (int) (stats.st_mtime));
                }
                len++;
                MySyncFile *f = malloc(sizeof(MySyncFile));
                f->lastEdit = stats.st_mtime;
                f->directoryIndex = i;
                strcpy(f->filename, dp->d_name);
                if (c->copyPermissions) {
                    f->permissions = stats.st_mode & PERMISSIONS;
                }
                strcpy(f->relativePath, path);
                strcat(f->relativePath, "/");
                strcat(f->relativePath, dp->d_name);
                list[listIdx++] = f;
            }
        }
        closedir(folder);
    }

}


MySyncFile **listAllFiles(Config *c) {
    MySyncFile *list[MAX_FILES_TO_COPY];
    MySyncFile *output[MAX_FILES_TO_COPY];
    char names[MAX_FILES_TO_COPY][MAX_PATH_LENGTH];
    int namesLength = 0;
    listAllFilesInternal(c, "", list);
    int len = 0;
    for (int i = 0; i < listIdx; i++) {
        MySyncFile *file = list[i];
        if (list[i] == NULL) break;
        bool shouldSkip = false;
        for (int k = 0; k < namesLength; k++) {
            if (strcmp(names[k], file->relativePath) == 0) {
                shouldSkip = true;
                break;
            }
        }
        if (shouldSkip
            || matchesRegex(c->excludePattern, file->filename)
            || (c->includePattern != NULL && !matchesRegex(c->includePattern, file->filename))) {
            continue;
        }
        strcpy(names[namesLength++], file->relativePath);

        MySyncFile *bestFile = list[i];
        for (int j = 0; j < listIdx; j++) {
            if (i == j) continue;
            if (strcmp(file->relativePath, list[j]->relativePath) != 0) continue;
            if (list[j]->lastEdit > bestFile->lastEdit) {
                bestFile = list[j];
            }
        }
        if (c->verboseMode) {
            printf("%s%s is the most up to date! it's permission number is %i\n",
                   c->directories[bestFile->directoryIndex],
                   bestFile->relativePath, bestFile->permissions);
        }
        output[len++] = bestFile;
    }


    MySyncFile **retFiles = malloc(sizeof(MySyncFile) * len + 1);
    for (int i = 0; i < len; i++) {
        retFiles[i] = output[i];
    }
    printf("\n\n\n");
    MySyncFile *f = malloc(sizeof(MySyncFile));
    strcpy(f->relativePath, "NONE");
    retFiles[len] = f;
    return retFiles;
}