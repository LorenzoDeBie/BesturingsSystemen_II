#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	// open /etc/passwd + error handling
	int fd = open("/etc/passwd", O_RDONLY);
	if(fd < 0) {
		perror(argv[0]);
		exit(1);
	}
	// lseek to the last ten bytes of the file + error handling
	if(lseek(fd, -10, SEEK_END) < 0) {
		perror(argv[0]);
		exit(1);
	}
	// buffer to hold last ten bytes
	unsigned char buf[10];
	// read last ten bytes in a buffer + error handling
	if( read(fd, buf, 10) < 0) {
		perror(argv[0]);
		exit(1);
	}
	// write the last ten bytes to the screen + error handling
	if( write(1, buf, 10) < 0) {
		perror(argv[0]);
		exit(1);
	}
	return 0;
}
