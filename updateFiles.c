//
// Created by healy on 11/10/2023.
//

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "updateFiles.h"
#include "find_files.h"
#include "FileManagement.h"

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
        updateFile(c, d[i]);
        i++;
    }
}