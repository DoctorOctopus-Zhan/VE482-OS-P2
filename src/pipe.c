#include "global.h"

int pipenum(int argc, char** argv) {
    int num = 0;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            ++num;
        }
    }
    return num;
}
