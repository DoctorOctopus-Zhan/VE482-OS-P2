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
    char infile[MAX_LENGTH];
    char outfile[MAX_LENGTH];
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
            if (i + 1 >= argc)
            {

                printf("> ");
                fflush(stdout);
                // scanf("%s", outfile);
                fgets(outfile, MAX_LENGTH, stdin);
                outfile[strlen(outfile) - 1] = '\0';
                rt.out_file = outfile;
            }
            else
            {
                rt.out_file = argv[i + 1];
            }
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
            if (i + 1 >= argc)
            {

                printf("> ");
                fflush(stdout);
                // scanf("%s", infile);
                fgets(infile, MAX_LENGTH, stdin);
                infile[strlen(infile) - 1] = '\0';
                rt.in_file = infile;
            }
            else
            {
                rt.in_file = argv[i + 1];
            }
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

    // if (isquote_miss)
    // {
    //     char temp[MAX_LENGTH];
    //     if (single_quote)
    //     {
    //     }
    //     else if (double_quote)
    //     {
    //         argv_new[argc_new-1][strlen(argv_new[argc_new-1])] = '\n';
    //         while (1)
    //         {
    //             bool complete = false;
    //             printf("> ");
    //             // char *temp = malloc(MAX_LENGTH * sizeof(char));
    //             memset(temp, 0, MAX_LENGTH);

    //             // memset(temp, 0, MAX_LENGTH);
    //             fgets(temp, MAX_LENGTH, stdin);
    //             printf("%s\n", temp);
    //             // fgets(argv_new[argc_new], MAX_LENGTH, stdin);
    //             // printf("%s\n", argv_new[argc_new]);
    //             // for (size_t i = 0; i < strlen(argv_new[argc_new]); ++i)
    //             // {
    //             //     if (argv_new[argc_new][i] == '"')
    //             //     {
    //             //         for (size_t k = i; k < strlen(argv_new[argc_new]); ++k)
    //             //         {
    //             //             argv_new[argc_new][k] = argv_new[argc_new][k + 1];

    //             //         }
    //             //         complete = true;
    //             //         break;
    //             //     }
    //             // }
    //             for (size_t i = 0; i < strlen(temp); ++i)
    //             {
    //                 if (temp[i] == '"')
    //                 {
    //                     // for (size_t k = i; k < strlen(temp); ++k)
    //                     // {
    //                     //     temp[k] = temp[k + 1];

    //                     // }
    //                     temp[i] = temp[i+1];
    //                     complete = true;
    //                     break;
    //                 }
    //             }

    //             temp[strlen(temp)-1] = '\0';
    //             // printf("%s\n", temp);
    //             argv_new[argc_new] = &temp[0];
    //             printf("%s\n", argv_new[argc_new]);

    //             // for (size_t i = 0; i < strlen(argv_new[argc_new]); ++i)
    //             // {
    //             //     printf("%s\n", argv_new[i]);
    //             //     if (argv_new[argc_new][i] == '"')
    //             //     {
    //             //         for (size_t k = i; k < strlen(argv_new[i]); ++k)
    //             //         {
    //             //             argv_new[i][k] = argv_new[i][k + 1];
    //             //         }
    //             //         printf("%s\n", argv_new[i]);
    //             //         complete = true;
    //             //     }
    //             // }

    //             // free(temp);
    //             ++argc_new;
    //             // for (int i = 0; i < argc_new; ++i)
    //             // {
    //             //     printf("%s\n", argv_new[i]);
    //             // }
    //             if (complete)
    //             {
    //                 break;
    //             }
    //         }
    //     }
    // }
    argv_new[argc_new] = NULL;

    if (argv_new[0] == NULL)
    {
        if (rt.io == 0)
        {
            printf("error: missing program\n");
            free(argv_new);
            return 1;
        }
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
            // free(argv_new);
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
            // perror(argv_new[0]);
            exit(1);
        }

        default:
        {
            // free(argv);

            int status;
            waitpid(pid, &status, 0);
        }
        }
    }
    free(argv_new);
    return 1;
}
