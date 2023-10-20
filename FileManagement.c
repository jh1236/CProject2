#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "FileManagement.h"


void updateFile(Config *c, MySyncFile *file, char *files) {
    char oldFilename[MAX_PATH_LENGTH];
    int idx = 0;
    sprintf(oldFilename, "%s%s", c->directories[file->directoryIndex], file->relativePath);
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c->directories[i] == NULL) break;
        if (i == file->directoryIndex) continue;
        char newFilename[MAX_PATH_LENGTH] = "";
        char currentPath[MAX_PATH_LENGTH] = "";
        char toBeCreated[MAX_PATH_LENGTH] = "";
        strcpy(toBeCreated, file->relativePath + 1);
        // create the required folders to the file
        // while there are more directories to be created
        while (strchr(toBeCreated, '/')) {
            // set the current path to the current path plus the next folder
            strcat(currentPath, strtok(toBeCreated, "/"));
            strcat(currentPath, "/");
            //set the new File name to be the root directory plus the relative directory
            strcpy(newFilename, c->directories[i]);
            strcat(newFilename, "/");
            strcat(newFilename, currentPath);
            struct stat st;
            // if the file doesn't exist
            if (stat(newFilename, &st) == -1) {
                if (c->shouldWrite) {
                    mkdir(newFilename, 0700);
                }
                if (c->verboseMode) {
                    printf("Creating directory %s\n", newFilename);
                }
            }
            // remove the part of to be created that has been created
            // as it no longer needs to be created because it was
            // already created in the creating part of the code
            strcpy(toBeCreated, strtok(NULL, ""));
        }
        // set up the new Filename variable to be the new filename (we used this for the folder)
        sprintf(newFilename, "%s%s", c->directories[i], file->relativePath);
        if (c->shouldWrite) {
            FILE *copyFrom = fopen(oldFilename, "r");
            FILE *copyTo = fopen(newFilename, "w");
            // copy file one to file two byte by byte
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
            sprintf(files, "%sCopying file %s to %s\n", files, oldFilename, newFilename);
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


void updateFiles(Config *c, MySyncFile **d) {
    int i = 0;
    // string to store list of files to copy, so we can print them again at the end to make them more clear.
    char files[MAX_FILES_TO_COPY * MAX_PATH_LENGTH] = "";
    while (strcmp(d[i]->relativePath, "NONE") != 0) {
        if (c->verboseMode) {
            printf("%s%s is most recent file\n", c->directories[d[i]->directoryIndex], d[i]->relativePath);
        }
        updateFile(c, d[i], files);
        i++;
    }
    if (c->verboseMode) {
        printf("\n\n\n");
        printf("%s", files);
        printf("\n\n\n");
    }
}