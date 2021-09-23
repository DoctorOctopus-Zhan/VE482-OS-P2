#include "global.h"

bool isreOutputAdd(char* str) {
    if (strcmp(str, ">>") == 0) {
        return true;
    }
    int n = (int)strlen(str);
    int left = 0, right = 1;
    while (right < n) {
        if (str[left++] == '>' && str[right++] == '>' && str[0] != '\'' && str[0] != '\"') {
            return true;
        }
    }
    return false;
}

bool isreOutput(char* str) {
    if (strcmp(str, ">") == 0) {
        return true;
    }
    int n = (int)strlen(str);
    int left = 0;
    while (left < n) {
        if (str[left++] == '>' && str[0] != '\'' && str[0] != '\"') {
            return true;
        }
    }
    return true;
}

bool isreInput(char* str) {
    if (strcmp(str, "<") == 0) {
        return true;
    }
    int n = (int)strlen(str);
    int left = 0;
    while (left < n) {
        if (str[left++] == '<' && str[0] != '\'' && str[0] != '\"') {
            return true;
        }
    }
    return true;
}
