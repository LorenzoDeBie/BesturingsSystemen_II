#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define N 100


/**
 * Now we try to use less memory and make it better
 * */
int main(int argc, char** argv) {
	int size = sysconf(_SC_PAGE_SIZE);
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
	// we need to open the output file in RDWR mode because mmap PROT_WRITE implies that we can also read from the file --> we need read on the fd
	if( (fd_out=open(argv[2], O_CREAT | O_RDWR, st.st_mode)) < 0) {
		close(fd_in);
		perror(argv[0]);
		return 1;
	}

	// we have to alter the size of the ouput file to the size we need
	// bus error = file is not big enough
	// option 1: lseek & write
	/* lseek(fd_out, st.st_size - 1, SEEK_SET);
	write(fd_out, "", 1); */
	// option 2: ftruncate = a lot cleaner
	ftruncate(fd_out, st.st_size);

	int total = 0, cpysz;
	while (total < st.st_size) {
		if(total < st.st_size - N * size) cpysz = N * size;
		else cpysz = st.st_size - total;
		char *in=mmap(NULL, cpysz, PROT_READ, MAP_PRIVATE, fd_in, total);
		if(in == MAP_FAILED) {
			perror(argv[0]);
			close(fd_in);
			close(fd_out);
			return 1;
		}
		
		char *out = mmap(NULL, cpysz, PROT_WRITE, MAP_SHARED, fd_out, total);
		if(out == MAP_FAILED) {
			perror(argv[0]);
			close(fd_in);
			close(fd_out);
			munmap(in, cpysz);
			return 1;
		}
		memcpy(out,in, cpysz);
		munmap(in, cpysz);
		munmap(out, cpysz);
		total += cpysz;
	}
	
	//close the fd's
	close(fd_in);
	close(fd_out);

	return 0;
}
