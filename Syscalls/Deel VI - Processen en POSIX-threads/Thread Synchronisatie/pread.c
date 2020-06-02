#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 2

/**
 * Exercise:
 * Write a program that reads the first 10*N bytes of /etc/passwd and writes them to stdout.
 * Do this by using N threads which each read 10 bytes.
 * Each thread writes these bytes to a file as well so we can see if the order is different
 * */
typedef struct {
	int fd;
	int fd_out;
	int offset;
} argument;

void* worker(void *p) {
	argument *arg = (argument*) p;
	unsigned char (*buf)[10] = malloc(sizeof(unsigned char) * 10);
	pread(arg->fd, buf, 10, arg->offset);
	printf("In thread: %s\n", *buf);
	// we have to use pwrite and pread because a combination of lseek with read or write is not thread safe:
	// if the thread gets interrupted between the lseek and read or write and another threads get executed fully
	// the fd will not be at the correct place --> wrong place is read/written to
	pwrite(arg->fd_out, buf, 10, arg->offset);
	return (void*) buf;
}

int main(int argc, char **argv) {
	pthread_t threads[N];
	argument args[N];
	//open /etc/password for reading
	int fd, fd_out, i;
	if( (fd = open("/etc/passwd", O_RDONLY)) < 0 ) {
		perror(argv[0]);
		exit(1);
	}
	if( (fd_out = open("test.txt", O_CREAT | O_WRONLY)) < 0 ) {
		perror(argv[0]);
		exit(1);
	}
	for(i = 0; i < N; i++) {
		args[i].fd = fd;
		args[i].offset = i * 10;
		args[i].fd_out = fd_out;
		pthread_create(&threads[i], NULL, worker,(void*) &args[i]);
	}
	printf("threads started!\n");
	
	unsigned char (*buf)[10];
	for(i = 0; i < N; i++) {
		pthread_join(threads[i],(void**) &buf);
		printf("In main: %s\n", *buf);
		free(buf);
	}

	close(fd);
	close(fd_out);
	return 0;
}
