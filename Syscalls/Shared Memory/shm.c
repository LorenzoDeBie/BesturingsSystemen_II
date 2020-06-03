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
	int *tab;
	// mmap the the shm object
	tab = mmap(NULL, 100*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if( tab == MAP_FAILED) {
		perror(argv[0]);
		return 1;
	}
	// fill the object with all ints from 0 to 99
	int i;
	for(i = 0; i < 100; i++) {
		tab[i] = i;
	}
	// sleep so we can watch /dev/shm/test
	sleep(60);
	// cleanup
	munmap(tab, sizeof(int)*100);
	shm_unlink("/test");
	return 0;
}
