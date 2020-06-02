#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if(argc != 3) {
		fprintf(stderr, "Usage cp <SOURCE> <DESTINATION>\n");
		return 1;
	}
	int fd_in, fd_out;
	if( (fd_in=open(argv[1], O_RDONLY)) < 0) {
		perror(argv[0]);
		return 1;
	}
	struct stat st;
	if( fstat(fd_in, &st) < 0) {
		close(fd_in);
		perror(argv[0]);
		return 1;
	}
	if( (fd_out=open(argv[2], O_CREAT | O_WRONLY, st.st_mode)) < 0) {
		close(fd_in);
		perror(argv[0]);
		return 1;
	}

	unsigned char buf[BUFSIZ];
	int n;
	while( (n = read(fd_in, buf, BUFSIZ)) != 0) {
		write(fd_out, buf, n);
	}
	
	close(fd_in);
	close(fd_out);
	return 0;
}
