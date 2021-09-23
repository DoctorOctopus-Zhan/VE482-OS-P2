#ifndef _GLOBAL_
#define _GLOBAL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_LENGTH 1024

char* readInput(void);
char** parse(char* line);
int execute(int argc, char* argv[]);

// Redirection

bool isreOutputAdd(char* str);
bool isreOutput(char* str);
bool isreInput(char* str);



#endif
