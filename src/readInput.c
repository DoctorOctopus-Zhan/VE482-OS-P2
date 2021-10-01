#include "global.h"

char* readInput(void) {
    char* line = malloc(MAX_LENGTH);
    // ctrlD = false;
    if (fgets(line, MAX_LENGTH, stdin) == NULL) {
        ctrlD = true;
        printf("exit\n");
        exit(0);
    }

    char* line_new = malloc(MAX_LENGTH);
    size_t index = 0;
    for (size_t i = 0; i < strlen(line)-1; ++i) {
        if (line[i] == '>' && line[i+1] == '>') {
            line_new[index++] = ' ';
            line_new[index++] = '>';
            line_new[index++] = '>';
            line_new[index++] = ' ';
            ++i;
        }
        else if (line[i] == '>') {
            line_new[index++] = ' ';
            line_new[index++] = '>';
            line_new[index++] = ' ';
        }
        else if (line[i] == '<') {
            line_new[index++] = ' ';
            line_new[index++] = '<';
            line_new[index++] = ' ';
        }
        else if (line[i] == '|') {
            line_new[index++] = ' ';
            line_new[index++] = '|';
            line_new[index++] = ' ';
        }
        else {
            line_new[index++] = line[i];
        }
    }
    free(line);
    // line[strlen(line)-1] = '\0';
    // return line;
    line_new[index] = '\0';
    return line_new;
}
