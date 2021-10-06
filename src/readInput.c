#include "global.h"

char *readInput(void)
{
    // char* line = malloc(MAX_LENGTH);
    char line[MAX_LENGTH];
    if (fgets(line, MAX_LENGTH, stdin) == NULL)
    {
        printf("exit\n");
        exit(0);
    }
    isquote = false;
    size_t len = strlen(line);
    // quotes handling
    int double_quote = 0, single_quote = 0;
    char *line_quote = malloc(MAX_LENGTH);
    size_t index = 0;
    for (size_t i = 0; i < len; ++i)
    {
        if (line[i] == '"')
        {
            isquote = true;
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
            isquote = true;
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

    line_quote[index] = '\0';
    len = index;

    char *line_new = malloc(MAX_LENGTH);
    index = 0;
    for (size_t i = 0; i < len - 1; ++i)
    {
        if (line_quote[i] == '>')
        {
            if (i < len - 1 && line_quote[i+1] == '>')
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
            // if (line[i-1] == '\\')
            // {
            //     line_new[index++] = '<';
            //     ++i;
            //     continue;
            // }
            line_new[index++] = ' ';
            line_new[index++] = '<';
            line_new[index++] = ' ';
        }
        else if (line_quote[i] == '|')
        {
            // if (line[i-1] == '\\')
            // {
            //     line_new[index++] = '|';
            //     ++i;
            //     continue;
            // }
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
    // free(line);
    // line[strlen(line)-1] = '\0';
    // return line;
    line_new[index] = '\0';
    free(line_quote);
    return line_new;
}
