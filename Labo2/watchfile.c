#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
	//cp src dest
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
	int aantal;
	char buffer[BUFSIZ];
	int lastEdit = st.st_mtime;
	while(1) {
		fstat(src, &st);
		//printf("old: %d; current: %d\n", lastEdit, st.st_mtim.tv_sec);
		if(lastEdit < st.st_mtime) {
			printf("file has changed!\n");
			lastEdit = st.st_mtime;
		}
	}
	return 0;
}	
