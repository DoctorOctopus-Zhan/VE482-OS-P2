#include "global.h"

int pipeNum(int argc, char** argv) {
    int num = 0;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            ++num;
        }
    }
    return num;
}

bool pipeExe(int argc, char** argv) {
    int num_pipe = pipeNum(argc, argv);
    if (num_pipe == 0) return false;
    int ith_pipe = 0;
    int left = 0, right = 0;
    int fd[2], in = 0;
    pid_t pid;

    char** argv_temp = malloc(MAX_LENGTH * sizeof(char*));
    while (ith_pipe <= num_pipe) {
        memset(fd, 0, 2);
        while (right < argc && strcmp(argv[right], "|") != 0) {
            ++right;
        }
        pipe(fd);
        
        for (int i = 0; i < right - left; ++i) {
            argv_temp[i] = argv[i + left];
        }
        argv_temp[right - left] = NULL;
        pid = fork();
        if (pid == 0) {
            // child process
            
            if (ith_pipe == 0) {
                dup2(fd[1], 1);
            }
            else if (ith_pipe == num_pipe) {
                dup2(in, 0);
                close(in);
            }
            else {
                dup2(in, 0);
                dup2(fd[1], 1);
                close(in);
            }
            execvp(argv[left], argv_temp);
            exit(1);
        }
        else if (pid < 0) {
            exit(1);
        }
        else {
            // int status;
            // waitpid(pid, &status, 0);
            // fflush(stdout);
            // close(fd[1]);
            // read()
        }
        ++ith_pipe;
        left = ++right;
        close(fd[1]);
        in = fd[0];
    }
    while (wait(NULL) != -1) {

    }
    free(argv_temp);
    return true;
}
