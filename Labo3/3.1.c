#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define AANTAL 3

int main(int argc, char** argv) {
    char *newargv[] = {"writestring", "hello", NULL};
    char *newenv[] = {NULL};
    int pids[AANTAL];
    int i = 0;
    for(i; i < AANTAL; i++) {
        pids[i] = fork();
        if(!pids[i]) {
            execve("./bin/writestring", newargv, newenv);
            perror("execve");
            exit(1);
        }
        /*else {
            int status;
            waitpid(pids[i], &status, 0);
            while(!WIFEXITED(status));
        }*/
    }
    //here is better because otherwise this process will wait after every execve
    for(i = 0; i < AANTAL; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        while(!WIFEXITED(status));
    }
    return 0;
}