#include "global.h"

char* readInput(void) {
    char* line = malloc(MAX_LENGTH);
    // ctrlD = false;
    if (fgets(line, MAX_LENGTH, stdin) == NULL) {
        ctrlD = true;
        printf("exit\n");
        exit(0);
    }

    line[strlen(line)-1] = '\0';
    return line;
}
