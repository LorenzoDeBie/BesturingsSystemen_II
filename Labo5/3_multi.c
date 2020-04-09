//
// Created by lorenzodb on 09/04/2020.
//

#include <stdio.h>
#include <pthread.h>

#define SIZE 1000
#define nThreads 8

int matA[SIZE][SIZE], matB[SIZE][SIZE], result[SIZE][SIZE] = {0};

typedef struct {
    int (*matA)[SIZE][SIZE];
    int (*matB)[SIZE][SIZE];
    int (*matRES)[SIZE][SIZE];
    int row;
} argument;

void calc_pos(const int (* const matA)[SIZE][SIZE], const int (* const matB)[SIZE][SIZE], int (* matRes)[SIZE][SIZE], int row, int col) {
    int i;
    for(i = 0; i < SIZE; i++) {
        (*matRes)[row][col] += (*matA)[row][i] * (*matB)[i][col];
    }
}

void calc_row(const int (* const matA)[SIZE][SIZE], const int (* const matB)[SIZE][SIZE], int (*matRes)[SIZE][SIZE], int row) {
    int i = 0;
    for(i = 0; i < SIZE; i++) {
        calc_pos(matA, matB, matRes, row, i);
    }
}

void* worker(void *a) {
    argument *arg = (argument *) a;
    calc_row(arg->matA, arg->matB, arg->matRES, arg->row);
    return 0;
}

int main(int argc, char** argv) {
    int row, col, currentRow = 0, i;
    pthread_t threads[nThreads];
    argument args[nThreads];

    for(row = 0; row < SIZE; row++) {
        for(col = 0; col < SIZE; col++) {
            matA[row][col] = row+col;
            matB[row][col] = row+col;
        }
    }

    //start all threads
    for(i = 0; i < nThreads; i++) {
        if(currentRow >= SIZE) break;
        args[i].matA = &matA;
        args[i].matB = &matB;
        args[i].matRES = &result;
        args[i].row = currentRow++;
        pthread_create(&threads[i], NULL, worker, (void *) &args[i]);
    }

    i=0;
    while(currentRow < SIZE) {
        pthread_join(threads[i], NULL);
        args[i].row = currentRow++;
        pthread_create(&threads[i], NULL, worker, (void *) &args[i]);
        i = (i + 1) % nThreads;
    }

    for(i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    //TODO: print matrix here

    return 0;
}

