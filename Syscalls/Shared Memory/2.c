#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 100
#define MAX 200

typedef struct {
	int numbers[N];
	int pids[N];
	int index;
	sem_t sem_CP;
	sem_t sem_PC;
} shared;

int main(int argc, char **argv) {
	int fd;
	if( (fd=shm_open("/test", O_CREAT|O_RDWR, (mode_t) 0666)) < 0) {
		perror(argv[0]);
		return 1;
	}
	// resize the shm object
	ftruncate(fd, sizeof(shared));
	shared *s = (shared*) mmap(NULL, sizeof(shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if( s == MAP_FAILED) {
		perror(argv[0]);
		return 1;
	}
	// mapping made -> fd can be closed
	close(fd);
	// init semaphores	
	// Child -> Parent: init to -N so that parent has to wait N times before it can search for the biggest number
	sem_init(&(s->sem_CP), 1, -N);
	// Parent -> child: start at 0 so all the childs will block untill parent is ready searching
	sem_init(&(s->sem_PC), 1, 0);
	int i;
	for(i = 0; i < N; i++) {
		int pid;
		if( (pid = fork()) < 0) {
			perror(argv[0]);
			// destroy semaphores
			sem_destroy(&(s->sem_PC));
			sem_destroy(&(s->sem_CP));
			// cleanup
			munmap(s,sizeof(shared));
			shm_unlink("/test");
			exit(1);
		}
		else if (pid == 0) {
			// CHILD
			// seed rand
			srand(getpid());
			// generate random number and save it in the struct
			s->numbers[i] = rand() % MAX;
			// signal parent
			sem_post(&(s->sem_CP));
			// wait for parent to signal the winner
			sem_wait(&(s->sem_PC));
			if(s->index == i) {
				printf("I'm the winner! pid: %d number: %d\n", getpid(), s->numbers[i]);
			}
			else {
				printf("Process %d is the winner. Number: %d\n", s->pids[s->index], s->numbers[i]);
			}
			exit(0);
		}
		// PARENT
		// save pid only in the parent!
		s->pids[i] = pid;
	}
	
	// PARENT
	// block on sem_CP
	sem_wait(&(s->sem_CP));
	// see which number is the biggest & set the index in the struct
	s->index = 0; // start value
	for(i = 0; i < N; i++)
		if(s->numbers[s->index] > s->numbers[i]) s->index = i;
	// signal the children
	for(i = 0; i < N; i++)
		sem_post(&(s->sem_PC));
	// wait for all children
	for( i = 0; i < N; i++) {
		waitpid(s->pids[i], NULL, 0);
	}
	// destroy semaphores
	sem_destroy(&(s->sem_PC));
	sem_destroy(&(s->sem_CP));
	// cleanup
	munmap(s,sizeof(shared));
	shm_unlink("/test");
	return 0;
}
