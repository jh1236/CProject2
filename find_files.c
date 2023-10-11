//
// Created by healy on 11/10/2023.
//

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include "find_files.h"

#include  <stdio.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <stdlib.h>

void list_directory(char *);

bool fileExists(char *filename) {
    if (access(filename, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

void list_all_files(Config c) {
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c.directories[i] == NULL) break;
        char* dir = c.directories[i];
        if (!fileExists(dir)) {
            perror("Error");
            exit(EXIT_FAILURE);
        } else {
            list_directory(dir);
        }
    }
}


void list_directory(char *dirname) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);
    if (dirp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    while ((dp = readdir(dirp)) != NULL) {
        printf("%s\n", dp->d_name);
    }
    closedir(dirp);
}