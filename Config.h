#ifndef PROJECT2_CONFIG_H
#define PROJECT2_CONFIG_H

#include <stdbool.h>

#define MAX_DIRECTORIES 10
#define MAX_INCLUDE_ARGUMENTS 10

typedef struct Config {
    bool allMode;
    bool shouldWrite;
    bool copyPermissions;
    bool recursive;
    bool verboseMode;
    char *includePattern[MAX_INCLUDE_ARGUMENTS];
    char *excludePattern[MAX_INCLUDE_ARGUMENTS];
    char *directories[MAX_DIRECTORIES];
} Config;

extern void printConfig(Config);
#endif //PROJECT2_CONFIG_H
