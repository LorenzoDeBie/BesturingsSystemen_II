#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define N 100

int main(int argc, char **argv) {
	// cat with stdin -> later
	if(argc < 2) {
		// now we read from stdin and print back what we get
		// no streams
		char buf[BUFSIZ];
		int n;
		while( (n = read(0, buf, BUFSIZ)) > 0) {
			write(1, buf, n);
		}
		return 0;

	}
	int size = sysconf(_SC_PAGE_SIZE);
	// loop over all the files and read one by one
	int i;
	for( i=1; i<argc; i++) {
		// open fd + error handling
		int fd;
		if( (fd = open(argv[i], O_RDONLY)) < 0) {
			perror(argv[0]);
			exit(1);
		}
		struct stat st;
		if( fstat(fd, &st) < 0) {
			perror(argv[0]);
			exit(1);
		}

		int total = 0, mapsize;
		char *buf;
		while( total < st.st_size ) {
			if( total < st.st_size - size * N ) {
				// still able to read full pagesize
				mapsize = size * N;
			}
			else {
				mapsize = st.st_size - total;
			}
			buf = mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, fd, total);
			if( buf == MAP_FAILED ) {
				close(fd);
				perror(argv[0]);
				exit(1);
			}
			printf("%s", buf);
			munmap(buf, mapsize);
			total += mapsize;
		}
		// close fd
		close(fd);
	}
	return 0;
}
