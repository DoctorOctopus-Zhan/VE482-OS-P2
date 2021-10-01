#include "global.h"

char* mypwd(void) {
    char* currPath = malloc(MAX_LENGTH * sizeof(char));
    getcwd(currPath, MAX_LENGTH);
    return currPath;
}

int mycd(int argc, char** argv, char* prePath) {
    
    if (argc == 1) {
        char* home = getenv("HOME");
        getcwd(prePath, MAX_LENGTH);
        return chdir(home);
    }
    if (strcmp(argv[1], "-") == 0) {
        if (prePath[0] == '\0') {
            printf("mumsh: cd: OLDPWD not set\n");
            return 0;
        }
        printf("%s\n", prePath);

        char temp[MAX_LENGTH];
        strcpy(temp, prePath);
        getcwd(prePath, MAX_LENGTH);
        return chdir(temp);
    }
    getcwd(prePath, MAX_LENGTH);

    
    if (strcmp(argv[1], ".") == 0) {
        return 0;
    }
    else if (strcmp(argv[1], "..") == 0) {
        char* currPath = mypwd();
        for (size_t i = strlen(currPath)-1; i > 0; --i) {
            if (currPath[i] != '/') {
                currPath[i] = '\0';
            }
            else {
                currPath[i] = '\0';
                break;
            }
        }
        chdir(currPath);
        free(currPath);
        return 0;
    }
    else if (strcmp(argv[1], "~") == 0) {
        return chdir("/root");
    }
    else {
        if (argv[1][0] == '/') {
            char absPath[MAX_LENGTH] = {'\0'};
            absPath[0] = '/';
            int index = 1;
            for (size_t i = 1; i < strlen(argv[1]); ++i) {
                if (argv[1][i] == '.' && argv[1][i+1] == '/') {
                    ++i;
                }
                else if (argv[1][i] == '.' && argv[1][i+1] == '.') {
                    absPath[--index] = '\0';
                    --index;
                    while (absPath[index] != '/') {
                        absPath[index--] = '\0';
                    }
                    ++index;
                    i += 2;
                }
                else {
                    absPath[index++] = argv[1][i];
                }
            }
            return chdir(absPath);
        }
        else {
            char absPath[MAX_LENGTH] = {'\0'};
            getcwd(absPath, MAX_LENGTH);
            size_t index = strlen(absPath);
            absPath[index++] = '/';
            
            for (size_t i = 0; i < strlen(argv[1]); ++i) {
                if (argv[1][i] == '.' && argv[1][i+1] == '/') {
                    ++i;
                }
                else if (argv[1][i] == '.' && argv[1][i+1] == '.') {
                    absPath[--index] = '\0';
                    --index;
                    while (absPath[index] != '/') {
                        absPath[index--] = '\0';
                    }
                    ++index;
                    i += 2;
                }
                else {
                    absPath[index++] = argv[1][i];
                }
            }
            return chdir(absPath);
        }
    }
    
    return -1;
}
