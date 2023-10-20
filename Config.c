#include "Config.h"
#include <stdio.h>

void printConfig(Config c) {
    printf("All Mode\t: %s\n", c.allMode ? "true" : "false");
    printf("Should Write\t: %s\n", c.shouldWrite ? "true" : "false");
    printf("Copy Perms\t: %s\n", c.copyPermissions ? "true" : "false");
    printf("Verbose Mode\t: %s\n", c.verboseMode ? "true" : "false");
    printf("Recursive Mode\t: %s\n", c.recursive ? "true" : "false");
    for (int i = 0; i < MAX_INCLUDE_ARGUMENTS; i++) {
        if (c.includePattern[i] == NULL) break;
        printf("Include Pattern\t: %s\n", c.includePattern[i]);
    }
    for (int i = 0; i < MAX_INCLUDE_ARGUMENTS; i++) {
        if (c.excludePattern[i] == NULL) break;
        printf("Exclude Pattern\t: %s\n", c.excludePattern[i]);
    }
    printf("Directories:\t:\n");
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c.directories[i] == NULL){
            break;
        }
        printf("\t\t %s\n", c.directories[i]);
    }
}