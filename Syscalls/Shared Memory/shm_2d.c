#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int fd;
	if( (fd=shm_open("/test", O_CREAT|O_RDWR, (mode_t) 0666)) < 0) {
		perror(argv[0]);
		return 1;
	}
	// resize the shm object
	ftruncate(fd, 100*sizeof(int));
	// pointer to array of (array of 10 ints)
	int (*tab)[10];
	// mmap the the shm object
	// if you stay inside the same process malloc is better
	// when you need to share this array with another process we need to use mmap
	tab = mmap(NULL, 100*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if( tab == MAP_FAILED) {
		perror(argv[0]);
		return 1;
	}
	// fill the object with all ints from 0 to 99
	int i, j;
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
			tab[i][j] = i*j;
		}
	}
	// sleep so we can watch /dev/shm/test
	sleep(60);
	// cleanup
	munmap(tab, sizeof(int)*100);
	shm_unlink("/test");
	return 0;
}
