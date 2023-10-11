#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "config.h"
#include "find_files.h"

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
            .excludePattern=NULL, .includePattern = NULL
    };
    int fileCount = 0;
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            switch (arg[1]) {
                case 'a':
                    config.allMode = true;
                    break;
                case 'i':
                    config.includePattern = argv[++i];   //increment i to avoid processing pattern as argument
                    break;
                case 'n':
                    config.shouldWrite = false;
                    config.verboseMode = true;
                    break;
                case 'o':
                    config.excludePattern = argv[++i];  //increment i to avoid processing pattern as argument
                    break;
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
            //TODO make this dynamic?
            if (fileCount >= MAX_DIRECTORIES - 1) {
                printf("That's too many directories!!\n");
                exit(EXIT_FAILURE);
            }
            config.directories[fileCount++] = arg;
        }
    }
    if (config.verboseMode) {
        printConfig(config);
    }
    list_all_files(config);
    return 0;
}
