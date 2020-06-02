#include <pthread.h>
#include <stdio.h>

// global vars get initialized to 0 in C
int a;

void* worker1(void *p) {
	a--;
}

void* worker2(void *p) {
	a++;
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
