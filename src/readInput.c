#include "global.h"

char* readInput(void) {
    char* line = malloc(MAX_LENGTH);
    fgets(line, MAX_LENGTH, stdin);
    int len = strlen(line);
    line[len-1] = '\0';
    return line;
}
