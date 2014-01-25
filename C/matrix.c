#include <stdlib.h>
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
    M->data = data;

    for (i = 0; i < rows; i++) {
        data[i] = &datap[i * cols];
    }

    return M;
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
