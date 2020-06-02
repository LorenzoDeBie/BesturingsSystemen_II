#include <pthread.h>
#include <stdio.h>

// global vars get initialized to 0 in C
int a;
// mutex = mutual exclusion
// we use a macro to init the mutex bcuz ez
// man pthread_mutex_init to see function it calls
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* worker1(void *p) {
	pthread_mutex_lock(&mtx);
	a--;
	pthread_mutex_unlock(&mtx);
}

void* worker2(void *p) {
	pthread_mutex_lock(&mtx);
	a++;
	pthread_mutex_unlock(&mtx);
}

int main(int argc, char** argv) {
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, worker1, NULL);
	pthread_create(&thread2, NULL, worker2, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("%d\n", a);

	return 0;
}
