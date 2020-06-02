#include <pthread.h>
#include <stdio.h>

#define N 4

void* worker(void *argument) {
	int *g = (int *)argument;
	int i;
	for(i = 0; i < 1000; i++) {
		printf("%d", g);
	}
	return;
}

int main() {
	pthread_t threads[N];
	int args[4] = {0,1,2,3};
	int i;
	for (i=0; i < N; i++) {
		pthread_create(&threads[i], NULL, worker, (void*) args[i]);
	}
	for(i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
