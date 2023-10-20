//  CITS2002 Project 2 2023
//  Student1:   23398223        Jared Healy
//  Student2:   21985625        Jacob Monteath

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "Config.h"
#include "FindFiles.h"
#include "GlobToRegex.h"
#include "FileManagement.h"

void usage(char name[]) {
    printf("Usage: %s [args] directory1  directory2  [directory3  ...]\n", name);
    printf("Available Arguments:\n");
    printf("\t-a\t\t:\tinclude hidden files\n");
    printf("\t-i pattern\t:\tignore files matching the pattern\n");
    printf("\t-n\t\t:\tidentify files to be updated, but don't do any writes\n");
    printf("\t-o pattern\t:\tonly include files matching the pattern\n");
    printf("\t-p\t\t:\tcopy file modification time and permissions\n");
    printf("\t-r\t\t:\trecursively search for files\n");
    printf("\t-v\t\t:\tverbose mode (log all output)\n");
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Too few Args\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    Config config = {
            .allMode = false, .copyPermissions=false, .shouldWrite=true, .recursive=false, .verboseMode=false,
            .excludePattern = {NULL}, .includePattern = {NULL}
    };
    int fileCount = 0;
    int includesCount = 0;
    int excludeCount = 0;
    //go through all the args
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            switch (arg[1]) {
                case 'a':
                    config.allMode = true;
                    break;
                case 'o': {
                    char *retv = glob2regex(argv[++i]);
                    if (retv == NULL) continue;
                    printf("%s\n", retv);
                    config.includePattern[includesCount++] = retv;
                    //increment i to avoid processing pattern as argument
                    break;
                }
                case 'n':
                    config.shouldWrite = false;
                    config.verboseMode = true;
                    break;
                case 'i': {
                    char *retv = glob2regex(argv[++i]);
                    if (retv == NULL) continue;
                    config.excludePattern[excludeCount++] = glob2regex(retv);
                    //increment i to avoid processing pattern as argument
                    break;
                }
                case 'p':
                    config.copyPermissions = true;
                    break;
                case 'r':
                    config.recursive = true;
                    break;
                case 'v':
                    config.verboseMode = true;
                    break;
                default:
                    printf("Unknown flag %s\n", arg);
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
            }
        } else {
            if (fileCount >= MAX_DIRECTORIES - 1) {
                printf("Too many directories!!\n");
                exit(EXIT_FAILURE);
            }
            config.directories[fileCount++] = arg;
        }
    }
    if (config.verboseMode) {
        printConfig(config);
        printf("\n\n\n");
    }
    MySyncFile **d = listAllFiles(&config);
    updateFiles(&config, d);
    //return the memory to its rightful owner
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (d[i] == NULL) break;
        if (strcmp(d[i]->relativePath, "NONE") == 0) {  //this is our 'NULL' file equivalent.
            free(d[i]);
            break;
        }
        free(d[i]);
        d[i] = NULL;
    }
    free(d);
    for (int j = 0; j < MAX_INCLUDE_ARGUMENTS; j++) {
        if (config.includePattern[j] == NULL) break;
        free(config.includePattern[j]);
    }
    for (int j = 0; j < MAX_INCLUDE_ARGUMENTS; j++) {
        if (config.excludePattern[j] == NULL) break;
        free(config.excludePattern[j]);
    }
    d = NULL;
    printf("Files Successfully synchronised!\n");
    return 0;
}
