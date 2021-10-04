#include "global.h"



int execute(int argc, char** argv) {
    
    pid_t pid;
    if (argv[0] == NULL) return 1;

    // check pipe
    if (pipeExe(argc, argv)) {
        return 1;
    }

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
        }
    }
    argv_new[argc_new] = NULL;

    // argc = argc_new;
    // for (int i = 0; i < argc; ++i) {
    //     argv[i] = argv_new[i];
    // }
    // argv[argc] = NULL;
    // free(argv_new);


    if (strcmp(argv_new[0], "exit") == 0) { // bulit-in exit
        printf("exit\n");
        exit(0);
    }
    else if (strcmp(argv_new[0], "pwd") == 0) { // built-in pwd
        pid = fork();
        if (pid == 0) {
            redirect_fd(&rt);
            char* currPath = mypwd();
            printf("%s\n", currPath);
            free(currPath);
            exit(1);
        }
        else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else if (strcmp(argv_new[0], "cd") == 0) { // built-in cd
        if (argc > 2) {
            printf("mumsh: cd: too many arguments\n");
            // free(argv_new);
            return 1;
        }
        
        int cd_success = mycd(argc_new, argv_new, prePath);
        
        if (cd_success == -1) {
            printf("mumsh: cd: %s: No such file or directory\n", argv_new[1]);
        }
        else if (cd_success == 0) {

        }
    }
    else {
        switch (pid = fork())
        {
        case -1:
            // exit(-1);
            break;

        case 0: {
            // signal(SIGINT, sigint_handler);
            redirect_fd(&rt);
            execvp(argv_new[0], argv_new);
            // execvp(argv[0], argv);
            exit(1);
        }


        default: {
            // free(argv);

            int status;
            waitpid(pid, &status, 0);

        }
        }
    }
    free(argv_new);
    return 1;
}

