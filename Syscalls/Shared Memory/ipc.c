#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct {
	int a, b;
	int som;
	sem_t sem_PC;
	sem_t sem_CP;
} shared;

int main(int argc, char **argv) {
	shared *s = mmap(NULL, sizeof(shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	sem_init(&(s->sem_PC), 1, 0);
	sem_init(&(s->sem_CP), 1, 0);
	int pid;

	if( (pid = fork()) < 0) {
		perror(argv[0]);
		return 1;
	}
	else if( pid == 0) {
		// child process
		// block untill parent wrote the two numbers
		sem_wait(&(s->sem_PC));
		s->som = s->a + s->b;
		// notify parent that sum has been calculated
		sem_post(&(s->sem_CP));
		return 0;
	}
	// parent proces
	char buffer[BUFSIZ];
	// read the input from stdin
	fgets(buffer, BUFSIZ, stdin);
	sscanf(buffer, "%d %d", &(s->a), &(s->b));
	// notify child that numbers are ready
	sem_post(&(s->sem_PC));
	// wait for child to calculate the sum
	sem_wait(&(s->sem_CP));
	printf("%d\n", s->som);
	waitpid(pid, NULL, 0);
	sem_destroy(&(s->sem_PC));
	sem_destroy(&(s->sem_CP));
	munmap(s, sizeof(shared));
	return 0;
}
