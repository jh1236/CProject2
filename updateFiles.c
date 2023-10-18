//
// Created by healy on 11/10/2023.
//

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "updateFiles.h"
#include "find_files.h"

bool fileExists(char *filename) {
    if (access(filename, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

void updateFiles(Config *c, MySyncFile **d) {
    int i = 0;
    while (strcmp(d[i]->relativePath, "NONE") != 0) {
        if (c->verboseMode) {
            printf("%s%s is most recent file\n", c->directories[d[i]->directoryIndex], d[i]->relativePath);
        }
        if (c->shouldWrite) {
            for (int j = 0; j < MAX_DIRECTORIES; j++) {
                if (c->directories[j] == NULL) break;
                if (0 == strcmp(c->directories[d[i]->directoryIndex], c->directories[j])) continue;
                printf("Copying from %s to %s\n", c->directories[d[i]->directoryIndex], c->directories[j]);
            }
        }
        i++;
    }
    printf("Im here!!\n");
}