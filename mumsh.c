
#include "global.h"

void sigint_handler(int signo)
{
    if (signo)
    {
        printf("\n");
    }

    if (jump_ctrlC == 0)
    {
        return;
    }
    siglongjmp(env, 26);
}

int main()
{

    char *line = NULL; // get a line of input
    int status;
    // int argc = 0; // numbers of command arguments
    char **argv = NULL;

    signal(SIGINT, sigint_handler);
    do
    {
        if (sigsetjmp(env, 1) == 26)
        {
            // jump here if no command but ctrl C
        }

        jump_ctrlC = 1;

        printf("mumsh $ ");
        fflush(stdout);

        line = readInput();

        argc = 0;

        jump_ctrlC = 0;
        argv = parse(line);
        status = execute(argc, argv);

        free(argv);
        free(line);
    } while (status);

    return 0;
}
