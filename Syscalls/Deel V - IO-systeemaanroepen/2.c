#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>

#define MAX 8192

int main(int argc, char **argv) {
	int i;
	char buffer[MAX];
	for(i=1; i<=MAX; i*=2) { 
		double start=clock();
		int fd=open("bigfile", O_RDONLY);
		if(fd<0) {
			perror(argv[0]);
			return 1;
		}
		//geen controlestructuur voor tijdsbesparing in les
		while(read(fd, &buffer, i)==i);
			
		if(close(fd)<0) {
			perror(argv[0]);
			return 1;
		}
		double time=(clock()-start)/CLOCKS_PER_SEC;
		printf("BUF_SIZ=\t %d\tTime=%f\n",i, time);
	}
	return 0;
}	
