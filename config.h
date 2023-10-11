//
// Created by healy on 11/10/2023.
//

#ifndef PROJECT2_CONFIG_H
#define PROJECT2_CONFIG_H

#include <stdbool.h>

#define MAX_DIRECTORIES 5


typedef struct Config {
    bool allMode;
    bool shouldWrite;
    bool copyPermissions;
    bool recursive;
    bool verboseMode;
    char *includePattern;
    char *excludePattern;
    char *directories[MAX_DIRECTORIES];
} Config;

extern void printConfig(Config);
#endif //PROJECT2_CONFIG_H
