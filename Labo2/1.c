#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	srand(time(0));
	int fd=open("bigfile", O_WRONLY|O_CREAT);
	if(fd<0) {
		perror(argv[0]);
		return 1;
	}
	int i=0;
	for(i;i<10*1024*1014; i++) {
		char ch;
		ch=rand()%26+'a';
		if(write(fd, &ch, 1)!=1) {
			perror(argv[0]);
			exit(1);
		}
	}
	if(close(fd)<0) {
		perror(argv[0]);
		return 1;
	}
	return 0;
}	
