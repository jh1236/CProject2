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

#define PERMISSIONS 511
// 777 in octal

bool matchesRegex(char *regexString, char *input) {
    if (regexString == NULL) return false;  //don't try and compare the string to null
    regex_t regex;
    regcomp(&regex, regexString, REG_EXTENDED);
    int match = regexec(&regex, input, 0, NULL, 0);
    return match == 0;
}


int listIdx = 0;

//Also IDK why this is warning me about recursion; that is on purpose clang...
void listAllFilesInternal(Config *c, char *path, MySyncFile **list) {
    char pathFromDirectory[MAX_PATH_LENGTH];
    int len = 0;
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c->directories[i] == NULL) break;  // we have reached the end of the list
        sprintf(pathFromDirectory, "%s%s", c->directories[i], path);
        DIR *folder;
        struct dirent *dp;
        folder = opendir(pathFromDirectory);
        if (folder == NULL) {
            perror("Error");
            continue;
        }
        while ((dp = readdir(folder)) != NULL) {
            //if the file is a hidden file and all mode is not enabled, continue
            if (dp->d_name[0] == '.' && !c->allMode) continue;
            struct stat stats;
            char fullPath[MAX_PATH_LENGTH * 2];

            // build the full path name
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
                    // add the folder to the working path of the next function
                    char prefix2[MAX_PATH_LENGTH];
                    strcpy(prefix2, path);
                    strcat(prefix2, "/");
                    strcat(prefix2, dp->d_name);
                    // run listAllFilesInternal again at the location of the new folder
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
                    //get the permissions out of st_mode by bitwise anding with 0o777
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
        // ---Check that we haven't already seen this file in another directory---
        for (int k = 0; k < namesLength; k++) {
            if (strcmp(names[k], file->relativePath) == 0) {
                shouldSkip = true;
                break;
            }
        }
        if (shouldSkip) {
            continue;
        }
        shouldSkip = c->includePattern[0] != NULL;
        // if the first element is null, the loop below won't run,
        // so we want the value to be true if there is a first element in the list
        // but false otherwise.
        // --- Check that this file matches all given include statements ---
        for (int j = 0; j < MAX_INCLUDE_ARGUMENTS; j++) {
            if (c->includePattern[j] == NULL) break;
            if (matchesRegex(c->includePattern[j], file->filename)) {
                shouldSkip = false;
                break;
            }
        }
        if (shouldSkip) {
            continue;
        }
        // --- Check that it isn't matching any exclude strings ---
        for (int j = 0; j < MAX_INCLUDE_ARGUMENTS; j++) {
            if (c->excludePattern[j] == NULL) break;
            if (matchesRegex(c->excludePattern[j], file->filename)) {
                shouldSkip = true;
                break;
            }
        }
        if (shouldSkip) {
            continue;
        }
        // add this filename to the list of checked names
        strcpy(names[namesLength++], file->relativePath);

        MySyncFile *bestFile = list[i];
        // find the newest version of this file
        for (int j = 0; j < listIdx; j++) {
            if (i == j) continue; // don't compare a file with itself
            if (strcmp(file->relativePath, list[j]->relativePath) != 0)
                continue; //if the filenames aren't the same, continue
            if (list[j]->lastEdit > bestFile->lastEdit) { // if the current file is not newer
                bestFile = list[j]; //replace it
            }
        }
        if (c->verboseMode) {
            printf("%s%s is the most up to date! it's permission number is %i\n",
                   c->directories[bestFile->directoryIndex],
                   bestFile->relativePath, bestFile->permissions);
        }
        output[len++] = bestFile;
    }

    // Turning your stack memory into heap memory with this one simple trick!!
    MySyncFile **retFiles = malloc(sizeof(MySyncFile) * len + 1);
    for (int i = 0; i < len; i++) {
        retFiles[i] = output[i];
    }
    // Add a NONE file at the end of the list, so we know when the list has ended
    MySyncFile *f = malloc(sizeof(MySyncFile));
    strcpy(f->relativePath, "NONE");
    retFiles[len] = f;
    return retFiles;
}