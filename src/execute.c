#include "global.h"

int execute(int argc, char** argv) {
    int temp = argc;
    temp = 0;
    pid_t pid;
    if (argv[0] == NULL) return 1;
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
            execvp(argv[0], argv);

            exit(1);
        }


        default: {
            int status;
            
            waitpid(pid, &status, 0);

        }
        }
    }
    return 1;
}

