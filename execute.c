#include "global.h"

int execute(int argc, char **argv)
{
    dup_in_re = false;
    dup_out_re = false;
    syntax_error = false;
    miss_pro = false;

    pid_t pid;
    if (argv[0] == NULL)
        return 1;

    // check pipe
    if (pipeExe(argc, argv))
    {
        return 1;
    }

    dup_in_re = false;
    dup_out_re = false;
    syntax_error = false;
    miss_pro = false;

    redirect_t rt;
    rt.io = 0;

    // check if there exists redirect
    // char infile[MAX_LENGTH];
    // char outfile[MAX_LENGTH];
    for (int i = 0; i < argc; ++i)
    {
        if (isreInput(argv[i]) || isreOutput(argv[i]) || isreOutputAdd(argv[i]))
        {
            if (i + 1 < argc)
            {
                if (isreInput(argv[i + 1]) || isreOutput(argv[i + 1]) || isreOutputAdd(argv[i + 1]))
                {
                    syntax_error = true;
                    syntax_error_token = argv[i + 1];
                    printf("syntax error near unexpected token `%s'\n", syntax_error_token);
                    return 1;
                }
            }
        }

        if (isreOutputAdd(argv[i]))
        {
            if (rt.io & 2 || rt.io & 4)
            {
                dup_out_re = true;
                printf("error: duplicated output redirection\n");
                return 1;
            }
            rt.io |= 4;
            rt.out_file = argv[i + 1];
        }
        if (isreOutput(argv[i]))
        {
            if (rt.io & 2 || rt.io & 4)
            {
                dup_out_re = true;
                printf("error: duplicated output redirection\n");
                return 1;
            }
            rt.io |= 2;          
            rt.out_file = argv[i + 1];
        }
        if (isreInput(argv[i]))
        {
            if (rt.io & 1)
            {
                dup_in_re = true;
                printf("error: duplicated input redirection\n");
                return 1;
            }
            rt.io |= 1;
            rt.in_file = argv[i + 1];
        }
    }

    // remove the redirection operators, in_file and out_file
    // copy argv into argv_new
    int argc_new = 0;
    char **argv_new = malloc(MAX_LENGTH * sizeof(char *));
    for (int i = 0; i < argc; ++i)
    {
        if (isreInput(argv[i]) || isreOutput(argv[i]) || isreOutputAdd(argv[i]))
        {
            ++i;
        }
        else
        {
            argv_new[argc_new++] = argv[i];
        }
    }

    // quote handle
    for (int i = 0; i < argc_new; ++i)
    {
        size_t index = 0;
        for (size_t j = 0; j < strlen(argv_new[i]); ++j)
        {
            if (argv_new[i][j] == '\\')
            {
                if (j < strlen(argv_new[i]) - 1)
                {
                    argv_new[i][index++] = argv_new[i][j + 1];
                    for (size_t k = j + 1; k < strlen(argv_new[i]); ++k)
                    {
                        argv_new[i][k] = argv_new[i][k + 1];
                    }
                }
            }
            else
            {
                // argv_new[i][index++] = argv_new[i][j];
                ++index;
            }
        }
    }

    
    argv_new[argc_new] = NULL;

    if (argv_new[0] == NULL)
    {
        printf("error: missing program\n");
        free(argv_new);
        return 1;
    }

    if (strcmp(argv_new[0], "exit") == 0)
    { // bulit-in exit
        printf("exit\n");
        exit(0);
    }
    else if (strcmp(argv_new[0], "pwd") == 0)
    { // built-in pwd
        pid = fork();
        if (pid == 0)
        {
            redirect_fd(&rt);
            char *currPath = mypwd();
            printf("%s\n", currPath);
            free(currPath);
            exit(1);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else if (strcmp(argv_new[0], "cd") == 0)
    { // built-in cd
        if (argc > 2)
        {
            printf("cd: too many arguments\n");
            return 1;
        }

        int cd_success = mycd(argc_new, argv_new, prePath);

        if (cd_success == -1)
        {
            printf("%s: No such file or directory\n", argv_new[1]);
        }
        else if (cd_success == 0)
        {
        }
    }
    else if (strcmp(argv_new[0], "jobs") == 0)
    {
        for (int i = 1; i <= job_num; ++i)
        {
            if (waitpid(job_pid[i - 1], NULL, WNOHANG) == 0)
            {
                printf("[%d] running %s\n", i, job[i - 1]);
            }
            else
            {
                printf("[%d] done %s\n", i, job[i - 1]);
            }
        }
    }
    else
    {
        switch (pid = fork())
        {
        case -1:
            // exit(-1);
            break;

        case 0:
        {
            open_non_exist = false;
            open_failed = false;
            redirect_fd(&rt);
            if (open_non_exist || open_failed || dup_in_re || dup_out_re || syntax_error || miss_pro)
            {
                exit(1);
            }

            execvp(argv_new[0], argv_new);

            printf("%s: command not found\n", argv_new[0]);
            exit(1);
        }

        default:
        {
            int status;
            if (isback)
            {
                job_pid[job_num - 1] = pid;
                strcpy(job[job_num - 1], line);
                printf("[%d] %s\n", job_num, line);
                waitpid(pid, NULL, WNOHANG);
            }
            else
            {
                waitpid(pid, &status, 0);
            }
        }
        }
    }
    free(argv_new);
    return 1;
}
