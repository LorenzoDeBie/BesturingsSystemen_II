//
// Created by lorenzodb on 05/04/2020.
//

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
    fork();
    fork();
    fork();
    printf("pid=%d\n", getpid());
    return 0;
}

