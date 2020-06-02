//
// Created by lorenzodb on 08/04/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define AANTAL 4
#define MAX 100

void* worker(void *arg) {
    int i, *num = (int *) arg;
    for(i = 0; i < 100; i++) {
        printf("%d : %d\n", i, *num);
    }
    return 0;
}

int main(int argc, char** argv) {
    pthread_t threads[AANTAL];
    int i;
    int numbers[AANTAL];

    srand(time(0));

    for(i = 0; i < AANTAL; i++) {
        numbers[i] = rand() % (MAX + 1);
        pthread_create(&threads[i], NULL, worker,(void *) &numbers[i]);
    }

    for(i = 0; i < AANTAL; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
