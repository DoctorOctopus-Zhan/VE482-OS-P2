#include "global.h"

bool ispipe(char *str)
{
    if (strcmp(str, "|") == 0)
    {
        return true;
    }
    return false;
}

int pipeNum(int argc, char **argv)
{
    int num = 0;
    for (int i = 0; i < argc; ++i)
    {
        if (ispipe(argv[i]))
        {
            ++num;
        }
    }
    return num;
}

bool pipeExe(int argc, char **argv)
{
    int num_pipe = pipeNum(argc, argv);
    if (num_pipe == 0)
        return false;

    int ith_pipe = 0;
    int left = 0, right = 0;

    // check errors
    while (ith_pipe <= num_pipe)
    {
        while (right < argc && strcmp(argv[right], "|") != 0)
        {
            ++right;
        }

        if (left == right)
        {
            printf("error: missing program\n");
            miss_pro = true;
            return true;
        }

        // check if there exists redirect
        redirect_t rt;
        rt.io = 0;
        for (int i = left; i < right; ++i)
        {
            if (isreInput(argv[i]) || isreOutput(argv[i]) || isreOutputAdd(argv[i]) || ispipe(argv[i]))
            {
                if (isreInput(argv[i + 1]) || isreOutput(argv[i + 1]) || isreOutputAdd(argv[i + 1]) || ispipe(argv[i + 1]))
                {
                    syntax_error = true;
                    syntax_error_token = argv[i + 1];
                    printf("syntax error near unexpected token `%s'\n", syntax_error_token);
                    return true;
                }
            }

            if (isreOutputAdd(argv[i]))
            {
                if (rt.io & 2 || rt.io & 4)
                {
                    dup_out_re = true;
                    printf("error: duplicated output redirection\n");
                    return true;
                }
                rt.io |= 4;
                // rt.out_file = argv[i + 1];
            }
            if (isreOutput(argv[i]))
            {
                if (rt.io & 2 || rt.io & 4)
                {
                    dup_out_re = true;
                    printf("error: duplicated output redirection\n");
                    return true;
                }
                rt.io |= 2;
                // rt.out_file = argv[i + 1];
            }
            if (isreInput(argv[i]))
            {
                if (rt.io & 1)
                {
                    dup_in_re = true;
                    printf("error: duplicated input redirection\n");
                    return true;
                }
                rt.io |= 1;
                // rt.in_file = argv[i + 1];
            }
        }

        if (ith_pipe == 0)
        {
            if (rt.io & 2 || rt.io & 4)
            {
                dup_out_re = true;
                printf("error: duplicated output redirection\n");
                return true;
            }
        }
        // the last pipe, change the input only
        else if (ith_pipe == num_pipe)
        {
            if (rt.io & 1)
            {
                dup_in_re = true;
                printf("error: duplicated input redirection\n");
                return true;
            }
        }
        else
        {
            if (rt.io & 1)
            {
                dup_in_re = true;
                printf("error: duplicated input redirection\n");
                return true;
            }
            if (rt.io & 2 || rt.io & 4)
            {
                dup_out_re = true;
                printf("error: duplicated output redirection\n");
                return true;
            }
        }
        ++ith_pipe;
        left = ++right;
    }

    ith_pipe = 0;
    left = 0;
    right = 0;

    int fd[2], in = 0; // in for connecting the previous pipe and the next pipe
    pid_t pid;

    char **argv_new = malloc(MAX_LENGTH * sizeof(char *));
    while (ith_pipe <= num_pipe)
    {
        memset(fd, 0, 2);
        while (right < argc && strcmp(argv[right], "|") != 0)
        {
            ++right;
        }

        // check if there exists redirect
        redirect_t rt;
        rt.io = 0;
        for (int i = left; i < right; ++i)
        {

            if (isreOutputAdd(argv[i]))
            {
                rt.io |= 4;
                rt.out_file = argv[i + 1];
            }
            if (isreOutput(argv[i]))
            {
                rt.io |= 2;
                rt.out_file = argv[i + 1];
            }
            if (isreInput(argv[i]))
            {
                rt.io |= 1;
                rt.in_file = argv[i + 1];
            }
        }

        // remove the redirection operators, in_file and out_file
        // copy argv into argv_new
        int argc_new = 0;
        for (int i = left; i < right; ++i)
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
        argv_new[argc_new] = NULL;

        for (int i = 0; i < argc_new; ++i)
        {
            size_t index = 0;
            for (size_t j = 0; j < strlen(argv_new[i]); ++j)
            {
                // printf("%c\n", argv_new[i][j]);
                if (argv_new[i][j] == '\\')
                {
                    if (j < strlen(argv_new[i]) - 1)
                    {
                        argv_new[i][index++] = argv_new[i][j + 1];
                        // printf("%s\n", argv_new[i]);
                        for (size_t k = j + 1; k < strlen(argv_new[i]); ++k)
                        {
                            argv_new[i][k] = argv_new[i][k + 1];
                        }
                        // printf("%s\n", argv_new[i]);
                    }
                }
                else
                {
                    // argv_new[i][index++] = argv_new[i][j];
                    ++index;
                }
            }
        }

        // if (argv_new[0] == NULL)
        // {
        //     printf("error: missing program\n");
        //     return true;
        // }

        pipe(fd);

        pid = fork();
        if (pid == 0)
        {
            // child process

            // the first pipe, change the output only
            if (ith_pipe == 0)
            {
                dup2(fd[1], 1);
                redirect_fd(&rt);
            }
            // the last pipe, change the input only
            else if (ith_pipe == num_pipe)
            {
                dup2(in, 0);
                redirect_fd(&rt);
            }
            else
            {
                dup2(in, 0);
                dup2(fd[1], 1);
            }
            // bulit-in pwd with pipe
            if (strcmp(argv[0], "pwd") == 0)
            {
                redirect_fd(&rt);
                char *currPath = mypwd();
                printf("%s\n", currPath);
                free(currPath);
                exit(1);
            }

            execvp(argv_new[0], argv_new);
            printf("%s: command not found\n", argv_new[0]);
            exit(1);
        }
        else if (pid < 0)
        {
            exit(1);
        }
        else
        {
            if (isback && ith_pipe == num_pipe)
            {
                job_pid[job_num - 1] = pid;
                strcpy(job[job_num - 1], line);
                printf("[%d] %s\n", job_num, line);
                waitpid(pid, NULL, WNOHANG);
            }
            else
            {
                while (wait(NULL) != -1)
                {
                }
                // int status;
                // waitpid(pid, &status, 0);
            }
        }
        ++ith_pipe;
        left = ++right;
        close(fd[1]);
        in = fd[0]; // record the previous pipe
    }
    // while (wait(NULL) != -1)
    // {
    // }
    free(argv_new);
    return true;
}
