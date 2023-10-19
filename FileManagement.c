//
// Created by healy on 19/10/2023.
//

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "FileManagement.h"

void updateFile(Config *c, MySyncFile *file) {
    char oldFilename[MAX_PATH_LENGTH];
    sprintf(oldFilename, "%s%s", c->directories[file->directoryIndex], file->relativePath);
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c->directories[i] == NULL) break;
        if (i == file->directoryIndex) continue;
        char newFilename[MAX_PATH_LENGTH] = "";
        char currentPath[MAX_PATH_LENGTH] = "";
        char toBeCreated[MAX_PATH_LENGTH] = "";
        strcpy(toBeCreated, file->relativePath + 1);
        while (strchr(toBeCreated, '/')) {
            strcat(currentPath, strtok(toBeCreated, "/"));
            strcat(currentPath, "/");
            strcpy(newFilename, c->directories[i]);
            strcat(newFilename, "/");
            strcat(newFilename, currentPath);
            struct stat st;
            if (stat(newFilename, &st) == -1) {
                if (c->shouldWrite) {
                    mkdir(newFilename, 0700);
                }
                if (c->verboseMode) {
                    printf("Creating directory %s\n", newFilename);
                }
            }
            strcpy(toBeCreated, strtok(NULL, ""));
        }
        sprintf(newFilename, "%s%s", c->directories[i], file->relativePath);
        if (c->shouldWrite) {
            FILE *copyFrom = fopen(oldFilename, "r");
            FILE *copyTo = fopen(newFilename, "w");
            int ch = fgetc(copyFrom);
            while (ch != EOF) {
                fputc(ch, copyTo);
                ch = fgetc(copyFrom);
            }
            fclose(copyFrom);
            fclose(copyTo);
        }
        if (c->verboseMode) {
            printf("Copying file %s to %s\n", oldFilename, newFilename);
        }
        if (c->copyPermissions) {
            if (c->shouldWrite) {
                chmod(newFilename, file->permissions);
            }
            if (c->verboseMode) {
                printf("Copying permission bytes %i\n", file->permissions);
            }
        }
    }
}