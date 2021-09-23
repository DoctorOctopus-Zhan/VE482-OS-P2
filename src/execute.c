#include "global.h"



int execute(int argc, char** argv) {
    int temp = argc;
    temp = 0;
    pid_t pid;
    if (argv[0] == NULL) return 1;

    redirect_t rt;
    rt.io = 0;

    // check if there exists redirect
    for (int i = 0; i < argc; ++i) {
        if (isreOutputAdd(argv[i])) {
            rt.io |= 4;
            rt.out_file = argv[i+1];
            
        }
        if (isreOutput(argv[i])) {
            rt.io |= 2;
            rt.out_file = argv[i+1];
        }
        if (isreInput(argv[i])) {
            rt.io |= 1;
            rt.in_file = argv[i+1];
        }
    }

    // remove the redirection operators, in_file and out_file
    // copy argv into argv_new
    int argc_new = 0;
    char** argv_new = malloc(MAX_LENGTH * sizeof(char*));
    for (int i = 0; i < argc; ++i) {
        if (isreInput(argv[i]) || isreOutput(argv[i]) || isreOutputAdd(argv[i])) {
            ++i;
        }
        else {
            argv_new[argc_new++] = argv[i];
            // printf("%s\n", argv_new[argc_new-1]);
        }
    }
    argv_new[argc_new] = NULL;

    if (strcmp(argv[0], "exit") == 0) {
        printf("exit\n");
        exit(0);
    }
    else {
        switch (pid = fork())
        {
        case -1:
            // exit(-1);
            break;

        case 0: {
            redirect_fd(&rt);
            execvp(argv_new[0], argv_new);
            // execvp(argv[0], argv);
            exit(1);
        }


        default: {
            int status;
            
            waitpid(pid, &status, 0);

        }
        }
    }
    free(argv_new);
    return 1;
}

