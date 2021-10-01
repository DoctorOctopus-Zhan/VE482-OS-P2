
#include "global.h"


int main() {
    char* line = NULL; // get a line of input
    int status;
    // int argc = 0; // numbers of command arguments
    char** argv = NULL;

    // char* prePath = malloc(MAX_LENGTH * sizeof(char));
    // memset(prePath, '\0', MAX_LENGTH * sizeof(char));
    char prePath[MAX_LENGTH] = {'\0'};

    do {

        printf("mumsh $ ");
        fflush(stdout);
        
        ctrlD = false;
        line = readInput();
        argc = 0;
        
        argv = parse(line);
        status = execute(argc, argv);

        free(argv);
        free(line);
    } while (status);
    free(prePath);
    return 0;
}
