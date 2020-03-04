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
	printf("lastEdit = %d\n", lastEdit);
	while(1) {
		if(stat(argv[1], &st) == 0) {
			if(lastEdit != st.st_mtime) {
				printf("file has changed!\n");
				lastEdit = st.st_mtime;
			}
		}
		else {
			perror(argv[1]);
			sleep(5);
		}
	}
	return 1;
}	
