#include "global.h"

char* readInput(void) {
    char* line = malloc(MAX_LENGTH);
    fgets(line, MAX_LENGTH, stdin);
    
    line[strlen(line)-1] = '\0';
    return line;
}
