#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>

#define MAX 8192

void vul_buffer(char *buffer, int n) {
	int i = 0;
	for(i; i < n; i++) {
		buffer[i] = rand()%26+'a';
	}
}

int main(int argc, char **argv) {
	srand(time(0));
	int i;
	char buffer[MAX];
	for(i=1; i<=MAX; i*=2) { 
		double start=clock();
		int fd=open("bigfile", O_WRONLY|O_CREAT);
		int tot = 0;
		if(fd<0) {
			perror(argv[0]);
			return 1;
		}

		while(tot < (10*1024*1024 - i)) {
			vul_buffer(buffer, i);
			tot += write(fd, buffer, i);
		}
		vul_buffer(buffer, 10*1024*1024-tot);
		write(fd, buffer, 10*1024*1024-tot);
		struct stat st;
		fstat(fd, &st);
		printf("SIZE=%d\n", st.st_size);	
			
		if(close(fd)<0) {
			perror(argv[0]);
			return 1;
		}
		double time=(clock()-start)/CLOCKS_PER_SEC;
		printf("BUF_SIZ=\t %d\tTime=%f\n",i, time);
		//delete bigfile
		unlink("bigfile");
	}
	return 0;
}	
