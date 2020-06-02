#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char **argv) {
	//cp src dest
	//give error when src or dest is not a file
	if(argc != 3) {
		printf("usage: cp src dest\n");
		exit(1);
	}
	struct stat st;
	//check src file
	if(stat(argv[1], &st)!= 0) {
		perror(argv[1]);
		exit(1);
	}
	if(S_ISDIR(st.st_mode)) {
		printf("%s is a directory!\n", argv[1]);
		exit(1);
	}
    struct stat st2;
    //check dest file
	if(stat(argv[2], &st2)==0) {
		printf("%s already exists!\n", argv[2]);
		exit(1);
	}
	if(S_ISDIR(st2.st_mode)) {
		printf("%s is a directory!\n", argv[2]);
		exit(1);
	}
	
	//now all files are good
	int src = open(argv[1], O_RDONLY);
	if(src < 0) {
		perror(argv[1]);
		exit(1);
	}
	int dest = open(argv[2], O_WRONLY|O_CREAT, st.st_mode);
	if(dest < 0) {
		perror(argv[2]);
		exit(1);
	}
	//all files are opened
	int aantal;
	char buffer[BUFSIZ];
	while((aantal=read(src,buffer,BUFSIZ))!=0) {
		write(dest, buffer,aantal); 
	}
	return 0;
}	
