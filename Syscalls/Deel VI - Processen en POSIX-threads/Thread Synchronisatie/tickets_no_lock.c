#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 8

int tickets = 1000000;

// why do it first without a mutex?
// We can only do parallelisation if we read tickets twice in asm
// one time in the while loop and one time with the -- operator 

void* worker(void *p) {
	int aantal;
	while(tickets > 0) {
		tickets--;
		aantal++;
	}
	// pthread_self() gives same output as pthread_create() (thread id)
	printf("%d %d\n", pthread_self(), aantal);
}

int main(int argc, char **argv) {
	pthread_t threads[N];
	int i;
	for(i = 0; i < N; i++) {
		pthread_create(&threads[i], NULL, worker, NULL);
	}
	for(i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
