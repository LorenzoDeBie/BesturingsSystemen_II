//
// Created by lorenzodb on 05/04/2020.
//

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    //give error when src or dest is not a file
    if(argc != 2) {
        printf("usage: watchfile path\n");
        exit(1);
    }
    struct stat st;
    //check src file
    if(stat(argv[1], &st)!= 0) {
        perror(argv[1]);
        exit(1);
    }
    if(!S_ISREG(st.st_mode)) {
        printf("%s is not a regular file!\n", argv[1]);
        exit(1);
    }

    //now all files are good
    int src = open(argv[1], O_RDONLY);
    if(src < 0) {
        perror(argv[1]);
        exit(1);
    }
    //all files are opened
    long lastEdit = st.st_mtime;
    printf("now watching %s; lastEdit = %d\n", argv[1], lastEdit);
    //keep reading while file exists
    while(stat(argv[1], &st) == 0) {
        if(lastEdit != st.st_mtime) {
            printf("file %s has changed!\n", argv[1]);
            lastEdit = st.st_mtime;
            sleep(3);
        }
    }
    //perror(argv[1]);
    return 0;
}
