#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_LENGTH 1024


char line[MAX_LENGTH]; // get a line of input

int argc; // numbers of command arguments
char* argv[MAX_LENGTH];
char cmd[MAX_LENGTH][MAX_LENGTH]; // command array : cmd[i] is a command


int readInput(char* line) {
    memset(line, 0, MAX_LENGTH);
    fgets(line, 1024, stdin);
    int len = strlen(line);
    line[len-1] = '\0';
    return strlen(line);
}

void parse(char* line) {
    argc = 0;
    for (int i = 0; i < MAX_LENGTH; ++i) {
        argv[i] = NULL;
        for (int j = 0; j < MAX_LENGTH; ++j) {
            cmd[i][j] = '\0';
        }
    }

    int len = strlen(line);

    int single_len = 0; // for recording line into cmd array
    for (int i = 0; i < len; ++i) {
        if (line[i] != ' ') {
            cmd[argc][single_len] = line[i];
            ++single_len;
        }
        else {
            if (single_len != 0) {
                cmd[argc][single_len] = '\0';
                single_len = 0;
                ++argc;
            }
        }
    }

    return;
}

void execute(int argc, char* argv[]) {
    if (strcmp(cmd[0], "exit") == 0) {
        printf("exit\n");
        exit(0);
    }
    return;
}

int main() {
    while (1) {
        printf("mumsh $ ");
        if (readInput(line) == 0) {
            continue;
        }
        parse(line);
        execute(argc, argv);
    }
    return 0;
}
