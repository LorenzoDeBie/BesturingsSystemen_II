#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 8

int tickets = 100;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* worker(void *p) {
	int aantal;
	while(pthread_mutex_lock(&mtx), tickets > 0) {
		tickets--;
		pthread_mutex_unlock(&mtx);
		aantal++;
		sleep(rand()%3);
	}
	pthread_mutex_unlock(&mtx);
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
