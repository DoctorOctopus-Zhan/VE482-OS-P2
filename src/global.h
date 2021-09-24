#ifndef _GLOBAL_
#define _GLOBAL_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <fcntl.h>

#define MAX_LENGTH 1024

int argc; // numbers of command arguments

bool ctrlD; // ctrl D input

typedef struct {
    int io; // 1 for input_rd, 2 for output_overwrite_rd, 4 for output_append_rd, 3 or 5
    // int out;
    char *in_file;
    char *out_file;
} redirect_t;

char* readInput(void);
char** parse(char* line);
int execute(int argc, char* argv[]);

// Redirection

bool isreOutputAdd(char* str);
bool isreOutput(char* str);
bool isreInput(char* str);

void redirect_in(char* in_file);
void redirect_out(char* out_file);
void redirect_outAdd(char* out_file);

void redirect_fd(redirect_t* rt);


#endif
