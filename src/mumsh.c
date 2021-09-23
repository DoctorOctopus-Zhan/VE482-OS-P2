
#include "global.h"


int main() {
    char* line = NULL; // get a line of input
    int status;
    // int argc = 0; // numbers of command arguments
    char** argv = NULL;

    do {

        printf("mumsh $ ");
        fflush(stdout);
        
        line = readInput();
        argc = 0;
        argv = parse(line);
        status = execute(argc, argv);

        free(argv);
        free(line);
    } while (status);
    return 0;
}
