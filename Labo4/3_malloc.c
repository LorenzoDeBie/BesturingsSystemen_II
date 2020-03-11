#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 50

int main(int argc, char** argv) {
    //eens proberen met 1 pipe
    int *pids;
    int **fds_C_P;
    int i, biggest = 0;
    if(argc != 2) {
        printf("Program expects only 1 parameter: number of childs to create!\n");
        return 1;
    }

    pids = malloc(sizeof(int)*atoi(argv[1]));
    fds_C_P = malloc(sizeof(int*) * atoi(argv[1]));

    for(i = 0; i < atoi(argv[1]); i++) {
        //make pipe for communications between child and parent
        fds_C_P[i] = malloc(sizeof(int) * 2);
        if(pipe(fds_C_P[i]) < 0) {
            perror(argv[0]);
            return 1;
        }
        //make child
        if((pids[i] = fork()) < 0) {
            perror(argv[0]);
            return 1;
        }
        if(pids[i] == 0) {
            int x = 0;
            //CHILD
            //create random number and write it to screen
            srand(getpid()); //use pid to seed rand to prevent children from generating same numbers
            unsigned int r = rand();
            //write result to pipe
            write(fds_C_P[i][1], &r, sizeof(r));
            close(fds_C_P[i][1]);
            printf("Child %d with pid %d generated: %u\n", i, getpid(), r);
            //free all the previous allocated arrays for fds
            for(x = 0; x <= atoi(argv[1]); x++) {
                free(fds_C_P[x]);
            }
            free(fds_C_P);
            free(pids);
            return 0;
        }
    }

    //read from all pipes
    for(i = 0; i < atoi(argv[1]); i++) {
        int n;
        read(fds_C_P[i][0], &n, sizeof(int));
        if(n > biggest) 
            biggest = n;
        //free this array of fd
        free(fds_C_P[i]);
    }
    free(fds_C_P);
    printf("Biggest number = %d\n", biggest);
    for(i = 0; i < atoi(argv[1]); i++) {
        waitpid(pids[i], NULL, 0);
    }
    free(pids);
    return 0;
}