//
// Created by healy on 11/10/2023.
//

#ifndef PROJECT2_FIND_FILES_H
#define PROJECT2_FIND_FILES_H

#include "config.h"
#define MAX_FILES_TO_COPY 200
#define MAX_PATH_LENGTH 255

typedef struct MySyncFile {
    long long lastEdit;
    char relativePath[MAX_PATH_LENGTH];
    char filename[MAX_PATH_LENGTH];
    int directoryIndex;
    int permissions;
} MySyncFile;

MySyncFile **listAllFiles(Config*);


#endif //PROJECT2_FIND_FILES_H
