#include "global.h"

bool isreOutputAdd(char* str) {
    if (strcmp(str, ">>") == 0) {
        return true;
    }
    // int n = (int)strlen(str);
    // int left = 0, right = 1;
    // while (right < n) {
    //     if (str[left++] == '>' && str[right++] == '>' && str[0] != '\'' && str[0] != '\"') {
    //         return true;
    //     }
    // }
    return false;
}

bool isreOutput(char* str) {
    if (strcmp(str, ">") == 0) {
        return true;
    }
    // int n = (int)strlen(str);
    // int left = 0;
    // while (left < n) {
    //     if (str[left++] == '>' && str[0] != '\'' && str[0] != '\"') {
    //         return true;
    //     }
    // }
    return false;
}

bool isreInput(char* str) {
    if (strcmp(str, "<") == 0) {
        return true;
    }
    // int n = (int)strlen(str);
    // int left = 0;
    // while (left < n) {
    //     if (str[left++] == '<' && str[0] != '\'' && str[0] != '\"') {
    //         return true;
    //     }
    // }
    return false;
}

void redirect_in(char* in_file) {
    int in_fd = open(in_file, O_RDONLY, 0666);
    if (in_fd == -1) {
        // perror(in_file);
        printf("%s: No such file or directory\n", in_file);
        open_non_exist = true;
        // close(in_fd);
        return;
    }
    dup2(in_fd, 0);
    close(in_fd);
    return;
}

void redirect_out(char* out_file) {
    int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1) {
        // perror(out_file);
        printf("%s: Permission denied\n", out_file);
        open_failed = true;
        return;
    }
    dup2(out_fd, 1);
    close(out_fd);
    return;
}

void redirect_outAdd(char* out_file) {
    int out_fd = open(out_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    dup2(out_fd, 1);
    close(out_fd);
    return;
}

void redirect_fd(redirect_t* rt) {
    switch (rt->io)
    {
    case 0:
        break;
    case 1:
        {
            redirect_in(rt->in_file);
            break;
        }
    case 2:
        {
            redirect_out(rt->out_file);
            break;
        }
    case 3:
        {
            redirect_in(rt->in_file);
            redirect_out(rt->out_file);
            break;
        }
    case 4:
        {
            redirect_outAdd(rt->out_file);
            break;
        }
    case 5:
        {
            redirect_in(rt->in_file);
            redirect_outAdd(rt->out_file);
            break;
        }
    default:
        // error handling
        break;
    }
}
