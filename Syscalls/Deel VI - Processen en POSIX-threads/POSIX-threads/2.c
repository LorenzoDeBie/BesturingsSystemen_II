//
// Created by lorenzodb on 08/04/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define SIZE 1000000
#define MAX 1000000

int array[SIZE] = {0};

typedef struct {
    int *array;
    int size;
    int (*f)(const int * const, int);
    int result;
} argument;

int find_min(const int * const array, int size) {
    int kleinste = array[0];
    int i;
    for(i = 0; i < size; i++)
        if(array[i] < kleinste) kleinste = array[i];
    return kleinste;
}

int find_max(const int * const array, int size) {
    int grootste = array[0];
    int i;
    for(i = 0; i < size; i++)
        if(array[i] > grootste) grootste = array[i];
    return grootste;
}

// worker function has to generic, no worker function for min & max --> give function min or max as argument
void* worker(void * a) {
    argument* arg = (argument*) a;
    arg->result =  arg->f(arg->array, arg->size);
    return 0;
}

int main(int argc, char** argv) {
    int i;

    srand(time(0));
    for(i = 0; i < SIZE; i++) {
        array[i] = rand() % MAX;
    }

    argument arg_min, arg_max;
    pthread_t threads[2];

    arg_min.array = array;
    arg_min.size = 5;
    arg_min.f = find_min;

    arg_max.array = array;
    arg_max.size = 5;
    arg_max.f = find_max;

    pthread_create(&threads[0], NULL, worker, (void*) &arg_min);
    pthread_create(&threads[1], NULL, worker, (void*) &arg_max);

    //return value is written to result field of argument, no need to save return value of worker function
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("min: %d; max: %d\n", arg_min.result, arg_max.result);

    return 0;
}
