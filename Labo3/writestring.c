#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("usage: writestring string; argc=%d\n", argc);
        return 1;
    }
    printf("pid=%d; %s\n", getpid(), argv[1]);
    sleep(20);
    return 0;
}