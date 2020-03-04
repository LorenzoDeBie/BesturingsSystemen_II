#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("pid van ouder: %d\n", getpid());
    int i = 0;
    for(i; i < 3; i++) {
        if(!fork()) {
            printf("pid van kind: %d\n", getpid());
            exit(0);
        }
    }
    return 0;
}