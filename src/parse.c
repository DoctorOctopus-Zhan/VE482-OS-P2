#include "global.h"

char** parse(char* line) {
    char** argv = malloc(MAX_LENGTH * sizeof(char*));

    int index = 0;
    char* token = strtok(line, " ");
    while (token != NULL) {
        argv[index++] = token;
        token = strtok(NULL, " ");
    }

    argv[index] = NULL;
    argc = index;
    return argv;
}
