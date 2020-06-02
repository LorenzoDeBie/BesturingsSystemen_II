#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Struct to hold all arguments for worker function
typedef struct {
	// array wich contains random numbers
	int *array;
	// size of the array
	int size;
	// function to execute on array with size
	int (*func)(int*, int);
} argument;

// find smallest value in the array
int kleinste(int * array, int n) {
	int kleinste = array[0];
	int i;
	for(i = 0; i < n; i++) {
		if (array[i]<kleinste) kleinste=array[i];
	}
	return kleinste;
}

// find biggest value in the array
int grootste(int *array, int n) {
	int grootste = array[0];
	int i;
	for(i=0; i < n; i++) {
		if(array[i]>grootste) grootste=array[i];
	}
	return grootste;
}

// worker function for threads
void* worker(void *a) {
	argument* arg=(argument*) a;
	int *ret = malloc(sizeof(int));
	*ret = (arg->func(arg->array, arg->size));
	return (void *) ret;
}

int main(int argc, char **argv) {
	// array of random numbers
	int array[5] = {1,7,-15,8,5};
	// threads to calc smallest and biggest
	pthread_t thread1, thread2;
	// structs to hold args
	argument a, b;
	// initialize the structs
	a.array = array;
	b.array = array;
	a.size = 5;
	b.size = 5;
	a.func = &kleinste;
	b.func = &grootste;
	// start threads
	pthread_create(&thread1, NULL, worker, (void*) &a);
	pthread_create(&thread2, NULL, worker, (void*) &b);
	// pointer to result
	// since the worker now uses malloc we cannot forget to free it here!
	int* ret;
	// wait for smallest
	pthread_join(thread1, (void**) &ret);
	printf("Kleinste: %d\n",*ret);
	// free the first allocated int
	free(ret);
	// wait for biggest
	pthread_join(thread2, (void**) &ret);
	// free the second allocated int
	printf("Grootste: %d\n",*ret);
	free(ret);
	return 0;
}
