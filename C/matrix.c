#include <stdlib.h>
#include "matrix.h"

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

void matrix_destructor(MATRIX * M)
{
    free(M->data[0]);
    free(M);
    return;
}
