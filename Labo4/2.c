#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    int fd_P_C1[2];
    int fd_P_C2[2];
    int fd_C1_C2[2];

    if(pipe(fd_P_C1) < 0) {
        perror(argv[0]);
        return 1;
    }
    if(pipe(fd_P_C2) < 0) {
        perror(argv[0]);
        return 1;
    }
    if(pipe(fd_C1_C2) < 0) {
        perror(argv[0]);
        return 1;
    }

    int pid1;
    if((pid1=fork()) < 0) {
        perror(argv[0]);
        return 1;
    }
    if(pid1==0) {
        //CHILD1
        close(fd_P_C2[0]);
        close(fd_P_C2[1]);
        close(fd_P_C1[1]);
        close(fd_C1_C2[0]);

        int n;
        char* buf[8192];
        while((n=read(fd_P_C1[0], buf, 8192)) != 0) {
            write(fd_C1_C2[1], buf, n);
        }
        close(fd_C1_C2[1]);
        return 0;
    }

    int pid2;
    if((pid2=fork()) < 0) {
        perror(argv[0]);
        return 1;
    }
    if(pid2==0) {
        //CHILD2
        close(fd_P_C1[0]);
        close(fd_P_C1[1]);
        close(fd_P_C2[1]);
        close(fd_C1_C2[1]);
        int n;
        char* buf[8192];
        while((n=read(fd_C1_C2[0], buf, 8192)) != 0) {
            write(1, buf, n);
        }
        while((n=read(fd_P_C2[0], buf, 8192)) != 0) {
            write(1, buf, n);
        }
        return 0;
    }

    //PARENT
    close(fd_C1_C2[0]);
    close(fd_C1_C2[1]);
    close(fd_P_C1[0]);
    close(fd_P_C2[0]);

    char* str1 = "Hallo ";
    char* str2 = "Lorenzo!\n";

    write(fd_P_C1[1], str1, strlen(str1));
    close(fd_P_C1[1]);
    write(fd_P_C2[1], str2, strlen(str2));
    close(fd_P_C2[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}