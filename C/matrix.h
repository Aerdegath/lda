#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct {
    double ** data;
    int rows, cols;
} MATRIX;

MATRIX * matrix_constructor(int rows, int cols);
void matrix_print(MATRIX *M, int decimals);
void matrix_destructor(MATRIX * M);
MATRIX * matrix_mean(MATRIX * M);

#endif
