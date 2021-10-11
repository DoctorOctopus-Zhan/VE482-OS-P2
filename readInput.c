#include "global.h"

bool issyntax_error(char *line)
{
    for (size_t i = 0; i < strlen(line) - 1; ++i)
    {
        if (line[i] == '>' || line[i] == '<' || line[i] == '|')
        {
            for (size_t j = i + 1; j < strlen(line) - 1; ++j)
            {
                if (line[j] == '>' || line[j] == '<' || line[j] == '|')
                {
                    return true;
                }
                else if (line[j] == ' ')
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            continue;
        }
    }
    return false;
}

bool isincomplete_redirect(char *line)
{
    if (strlen(line) <= 1) return false;
    for (size_t i = strlen(line) - 2; i >= 0; --i)
    {
        if (line[i] == ' ' || line[i] == '\0')
            continue;
        else if (line[i] == '>' || line[i] == '<' || line[i] == '|')
        {
            if (i > 0)
            {
                if (line[i - 1] == '\\') return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

char *readInput(void)
{
    // background
    isback = false;

    // incomplete
    isincomplete = false;


    // char* line = malloc(MAX_LENGTH);
    // char line[MAX_LENGTH];
    if (fgets(line, MAX_LENGTH, stdin) == NULL)
    {
        printf("exit\n");
        exit(0);
    }

    // syntax_error
    syntax_error = issyntax_error(line);

    isquote_miss = false;
    size_t len = strlen(line);
    // quotes handling
    double_quote = 0;
    single_quote = 0;
    char *line_quote = malloc(MAX_LENGTH);
    size_t index = 0;
    for (size_t i = 0; i < len; ++i)
    {
        if (line[i] == '&')
        {
            isback = true;
            ++job_num;
            continue;
        }
        if (line[i] == '"')
        {
            ++i;
            ++double_quote;
            while (i < len && line[i] != '"')
            {
                if (line[i] == '>' || line[i] == '<' || line[i] == '|')
                {
                    line_quote[index++] = '\\';
                    line_quote[index++] = line[i++];
                }
                else
                {
                    line_quote[index++] = line[i++];
                }
            }
            if (i < len)
            {
                --double_quote;
            }
            continue;
        }

        if (line[i] == '\'')
        {

            ++i;
            ++single_quote;
            while (i < len && line[i] != '\'')
            {
                if (line[i] == '>' || line[i] == '<' || line[i] == '|')
                {
                    line_quote[index++] = '\\';
                    line_quote[index++] = line[i++];
                }
                else
                {
                    line_quote[index++] = line[i++];
                }
            }
            if (i < len)
            {
                --single_quote;
            }
            continue;
        }
        line_quote[index++] = line[i];
    }

    if (single_quote || double_quote)
    {
        isquote_miss = true;
    }

    line_quote[index] = '\0';
    len = index;

    isincomplete = isincomplete_redirect(line_quote);

    char *line_new = malloc(MAX_LENGTH);
    index = 0;
    for (size_t i = 0; i < len - 1; ++i)
    {
        if (line_quote[i] == '>')
        {
            if (i < len - 1 && line_quote[i + 1] == '>')
            {
                line_new[index++] = ' ';
                line_new[index++] = '>';
                line_new[index++] = '>';
                line_new[index++] = ' ';
                ++i;
                continue;
            }
            line_new[index++] = ' ';
            line_new[index++] = '>';
            line_new[index++] = ' ';
        }
        else if (line_quote[i] == '<')
        {
            line_new[index++] = ' ';
            line_new[index++] = '<';
            line_new[index++] = ' ';
        }
        else if (line_quote[i] == '|')
        {
            line_new[index++] = ' ';
            line_new[index++] = '|';
            line_new[index++] = ' ';
        }
        else if (line_quote[i] == '\\')
        {
            line_new[index++] = line_quote[i++];
            line_new[index++] = line_quote[i];
        }
        else
        {
            line_new[index++] = line_quote[i];
        }
    }
    line_new[index] = '\0';

    if (isquote_miss || isincomplete)
    {
        char temp[MAX_LENGTH];
        if (single_quote)
        {
            while (1)
            {
                line_new[index++] = '\n';
                bool complete = false;
                printf("> ");
                fflush(stdout);
                fgets(temp, MAX_LENGTH, stdin);

                for (size_t i = 0; i < strlen(temp) - 1; ++i)
                {
                    if (temp[i] == '\'')
                    {
                        ++single_quote;
                    }
                    else
                    {
                        if (temp[i] == '>' || temp[i] == '<' || temp[i] == '|')
                        {
                            if (single_quote % 2 != 0)
                            {
                                line_new[index++] = '\\';
                                line_new[index++] = temp[i];
                                continue;
                            }
                        }
                        line_new[index++] = temp[i];
                    }
                }

                line_new[index] = '\0';

                if (single_quote % 2 == 0)
                {
                    complete = true;
                }
                if (complete)
                {
                    isincomplete = isincomplete_redirect(line_new);
                    break;
                }
            }
        }
        else if (double_quote)
        {
            while (1)
            {
                line_new[index++] = '\n';
                bool complete = false;
                printf("> ");
                fflush(stdout);
                fgets(temp, MAX_LENGTH, stdin);

                for (size_t i = 0; i < strlen(temp) - 1; ++i)
                {
                    if (temp[i] == '"')
                    {
                        ++double_quote;
                    }
                    else
                    {
                        if (temp[i] == '>' || temp[i] == '<' || temp[i] == '|')
                        {
                            if (double_quote % 2 != 0)
                            {
                                line_new[index++] = '\\';
                                line_new[index++] = temp[i];
                                continue;
                            }
                        }
                        line_new[index++] = temp[i];
                    }
                }
                line_new[index++] = ' ';
                line_new[index] = '\0';

                if (double_quote % 2 == 0)
                {
                    complete = true;
                }
                if (complete)
                {
                    isincomplete = isincomplete_redirect(line_new);                    
                    break;
                }
            }
        }

        if (isincomplete && !syntax_error)
        {
            while (isincomplete)
            {
                line_new[index++] = ' ';
                printf("> ");
                fflush(stdout);
                fgets(temp, MAX_LENGTH, stdin);
                for (size_t i = 0; i < strlen(temp) - 1; ++i)
                {
                    if (temp[i] == '"' || temp[i] == '\'')
                    {
                    }
                    else
                    {
                        line_new[index++] = temp[i];
                    }
                }
                line_new[index++] = ' ';
                line_new[index] = '\0';
                isincomplete = isincomplete_redirect(line_new);                
            }            
        }
    }

    free(line_quote);
    return line_new;
}
