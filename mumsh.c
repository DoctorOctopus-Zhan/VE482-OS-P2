#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
    while (1) {
        printf("mumsh $ ");
        char cmd[1024];
        fgets(cmd, 1024, stdin);
        if (strcmp(cmd, "exit\n") == 0) {
            printf("exit\n");
            break;
        }
    }
    return 0;
}
