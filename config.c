//
// Created by healy on 11/10/2023.
//

#include "config.h"
#include <stdio.h>

void printConfig(Config c) {
    printf("All Mode\t: %s\n", c.allMode ? "true" : "false");
    printf("Should Write\t: %s\n", c.shouldWrite ? "true" : "false");
    printf("Copy Perms\t: %s\n", c.copyPermissions ? "true" : "false");
    printf("Verbose Mode\t: %s\n", c.verboseMode ? "true" : "false");
    printf("Recursive Mode\t: %s\n", c.recursive ? "true" : "false");
    if (c.includePattern != NULL) {
        printf("Include Pattern\t: %s", c.includePattern);
    }
    if (c.excludePattern != NULL) {
        printf("Exclude Pattern\t: %s", c.includePattern);
    }
    printf("Directories:\t:\n");
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (c.directories[i] == NULL){
            break;
        }
        printf("\t\t %s\n", c.directories[i]);
    }
}