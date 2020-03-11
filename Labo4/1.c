#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    int fds[2];
    int pid;
    if(pipe(fds) < 0) {
        perror(argv[0]);
        exit(1);
    }
    if((pid=fork()) < 0) {
        perror(argv[0]);
        exit(1);
    }
    if(pid==0) {
        //CHILD
        close(fds[1]); //close pipe in, child only reads
        int n;
        char buffer[8192];
        //if parent doesn't close write end of pipe this will keep waiting and not recieve 0
        while ((n=read(fds[0], buffer, 8192)) != 0) {
            //write is better than for loop with printf
            write(1, buffer, n);
            //write char per char because we don't know buffer ends with null byte
            /*
            int i;
            for(i = 0; i < n; i++) {
                printf("%c", buffer[i]);
            }
            */
        }
        //close(fds[0]); //dit is niet nodig
        exit(0);
    }
    //PARENT
    close(fds[0]); // close read end of pipe, parent only writes to pipe
    char *s ="Hallo\n";
    write(fds[1], s, strlen(s));
    //close write end of pipe so child gets 0 if it reads  
    close(fds[1]);
    waitpid(pid, NULL, 0);
    return 0;
}