#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 8

int tickets = 100;

sem_t sem;

void* worker(void *p) {
	int aantal;
	//sem_wait() decreases the semaphore by one or locks
	while(sem_wait(&sem), tickets > 0) {
		tickets--;
		// sem_post increases the semaphore by one or unlocks it
		sem_post(&sem);
		aantal++;
		sleep(rand()%3);
	}
	sem_post(&sem);
	// pthread_self() gives same output as pthread_create() (thread id)
	printf("%d %d\n", pthread_self(), aantal);
}

int main(int argc, char **argv) {
	//init the semaphore: shared = 0 (not between processes), start value = 1 => binary semaphore
	sem_init(&sem, 0, 1);
	pthread_t threads[N];
	int i;
	for(i = 0; i < N; i++) {
		pthread_create(&threads[i], NULL, worker, NULL);
	}
	for(i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}
	sem_destroy(&sem);
	return 0;
}
