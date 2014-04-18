#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

/*
 * Allocates and returns a MATRIX * object
 * rows, cols: number of rows, columns to be allocated
 */
MATRIX * matrix_constructor(int rows, int cols)
{
    int i;
    MATRIX * M = (MATRIX *) malloc(sizeof(MATRIX));
    double ** data = (double **) malloc(rows * sizeof(double *));
    double * datap = (double *) malloc(rows * cols * sizeof(double));
    M->rows = rows;
    M->cols = cols;

    //set each data pointer to the first element in each row
    for (i = 0; i < rows; i++) {
        data[i] = &datap[i * cols];
    }

    M->data = data;

    return M;
}

/*
 * Print function
 */
void matrix_print(MATRIX *M, int decimals)
{
    int i, j;

    for(i = 0; i < M->rows; i++) {
        for(j = 0; j < M->cols; j++) {
            printf("%12.*lf", decimals, M->data[i][j]);
        }
        printf("\n");
    }
}

/*
 * Frees the MATRIX * object
 * M: the MATRIX * to be destroyed
 */
void matrix_destructor(MATRIX * M)
{
    free(*M->data);
    free(M->data);
    free(M);
    return;
}
