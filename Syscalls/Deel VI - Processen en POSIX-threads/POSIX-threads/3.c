//
// Created by lorenzodb on 09/04/2020.
//

#include <stdio.h>

#define SIZE 1000
#define nThreads 8

int matA[SIZE][SIZE], matB[SIZE][SIZE], result[SIZE][SIZE] = {0};

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

int main(int argc, char** argv) {
    int row, col, i;

    for(row = 0; row < SIZE; row++) {
        for(col = 0; col < SIZE; col++) {
            matA[row][col] = row+col;
            matB[row][col] = row+col;
        }
    }

    for(i = 0; i < SIZE; i++) {
        calc_row(&matA, &matB, &result, i);
    }

    //TODO: print matrix here

    return 0;
}

