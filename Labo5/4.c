//
// Created by lorenzodb on 09/04/2020.
//

#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000000

int tab1[SIZE] = {0};
int tab2[SIZE] = {0};

void fill_reverse(int *tab) {
    int i;
    for(i = 0; i < SIZE; i++) {
        tab[i] = SIZE - i;
    }
}

void merge(int *tab, int size) {
    int *help = malloc(size * sizeof(int));
    int i = 0, j = size/2, counter = 0;

    while(i < size/2 && j < size) {
        if(tab[i] <= tab[j]) {
            help[counter++] = tab[i++];
        }
        else {
            help[counter++] = tab[j++];
        }
    }

    while(i < size/2) {
        help[counter++] = tab[i++];
    }

    //copy it all back from help
    for(i = 0; i < j; i++) {
        tab[i] = help[i];
    }

    free(help);
}

void sort(int *tab, int size) {
    if(size>1) {
        sort(tab, size/2);
        sort(tab+size/2, size-size/2);
        merge(tab, size);
    }
}

void* worker(void *a) {
    int *tab = (int*) a;
    fill_reverse(tab);
    sort(tab, SIZE);
    return 0;
}

int main(int argc, char** argv) {
    //no multithreading
    /*
    fill_reverse(tab1, SIZE);
    fill_reverse(tab2, SIZE);

    sort(tab1, SIZE);
    sort(tab2, SIZE);
     */

    //multithreading
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, worker, (void *) tab1);
    pthread_create(&threads[1], NULL, worker, (void *) tab2);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}