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
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_LENGTH 1024

int argc; // numbers of command arguments

char prePath[MAX_LENGTH]; // record the previous dir path

static sigjmp_buf env; // ctrl C

static volatile sig_atomic_t jump_ctrlC;

typedef struct
{
    int io; // 1 for input_rd, 2 for output_overwrite_rd, 4 for output_append_rd, 3 or 5
    char *in_file;
    char *out_file;
} redirect_t;

char *readInput(void);
char **parse(char *line);
int execute(int argc, char *argv[]);

// Redirection

bool isreOutputAdd(char *str);
bool isreOutput(char *str);
bool isreInput(char *str);

void redirect_in(char *in_file);
void redirect_out(char *out_file);
void redirect_outAdd(char *out_file);

void redirect_fd(redirect_t *rt);

// pipe

bool ispipe(char *str);
int pipeNum(int argc, char **argv);
bool pipeExe(int argc, char **argv);

// bulit-in command

char *mypwd(void);
int mycd(int argc, char **argv, char prePath[MAX_LENGTH]);

// ctrl C

void sigint_handler(int signo);

// error

bool open_non_exist;

bool open_failed;

bool dup_in_re;

bool dup_out_re;

bool syntax_error;
char *syntax_error_token;

bool miss_pro;

#endif
