/*******************************************************************************
 Use Principle Component Analysis (PCA) and Fisher Linear Discriminant (FLD) to determine the most
 discriminating features between images of faces.

 Description: This function gets a 2D matrix, containing all training image vectors
 and returns 4 outputs which are extracted from training database.
 Suppose Ti is a training image, which has been reshaped into a 1D vector.
 Also, P is the total number of MxN training images and C is the number of
 classes. At first, centered Ti is mapped onto a (P-C) linear subspace by V_PCA
 transfer matrix: Zi = V_PCA * (Ti - m_database).
 Then, Zi is converted to Yi by projecting onto a (C-1) linear subspace, so that
 images of the same class (or person) move closer together and images of difference
 classes move further apart: Yi = V_Fisher' * Zi = V_Fisher' * V_PCA' * (Ti - m_database)

 Argument:
    D                             - ((M*N)xP) A 2D matrix, containing all 1D image vectors.
                                     All of 1D column vectors have the same length of M*N,
                                     and 'D' will be a MNxP 2D matrix.

 Returns:
    M                             - MATRIX ** consisting of the following 4 entries:
    M[0] = mean                   - ((M*N)x1) Mean of the training database
    M[1] = V_PCA                  - ((M*N)x(P-C)) Eigen vectors of the covariance matrix of the training database
    M[2] = V_Fisher               - ((P-C)x(C-1)) Largest (C-1) eigen vectors of matrix J = inv(Sw) * Sb
    M[3] = ProjectedImages_Fisher - ((C-1)xP) Training images, which are projected onto Fisher linear space

 See also: EIG

 Original version by Amir Hossein Omidvarnia, October 2007
                     Email: aomidvar@ece.ut.ac.ir
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cblas.h>
#include <lapacke.h>
#include <assert.h>

#include "ppm.h"
#include "CreateDatabase.h"
#include "matrix.h"
#include "FisherfaceCore.h"

MATRIX **FisherfaceCore(const database_t *Database)
{
    //int Class_population = 4; //Set value according to database (Images per person)
    //int C = D->images / Class_population; //Number of classes (or persons)
    int P = Database->images; //Total Number of training images
    int i, j;
    //int INFO; //Return value for LAPACK eigen function
    double temp = 0;
    MATRIX **M; //What the function returns
    MATRIX *mean; //Mean matrix
    MATRIX *A; //Deviation matrix
    MATRIX *L; //Surrogate of covariance matrix, L = A_trans * A
    MATRIX *D; //Eigenvalues
    MATRIX *V; //Eigenvectors

    M = (MATRIX **) malloc(4 * sizeof(MATRIX *));


//    printf("Database:\n");
//    for (i = 0; i < WIDTH * HEIGHT; i++) {
//        for (j = 0; j < P; j++) {
//            printf("%6.0f", Database->data[i][j]);
//        }
//        printf("\n");
//    }


    //**************************************************************************
    //Calculate mean
    //<.m: 36>
    mean = matrix_constructor(Database->pixels, 1);

    //Calculate Mean matrix
    for (i = 0; i < Database->pixels; i++) {
        temp = 0;
        for (j = 0; j < P; j++) {
            temp += (double) Database->data[i][j];
        }
        mean->data[i][0] = (temp / P);
    }

    //Assign mean database
    M[0] = mean;

    //printf("\nmean:\n");
    //matrix_print(M[0]);

    //**************************************************************************
    //Calculate A, deviation matrix
    //<.m: 39>
    A = matrix_constructor(Database->pixels, P);

    for (i = 0; i < Database->pixels; i++) {
        // each column in A->data is the difference between an image and the mean
        for (j = 0; j < P; j++) {
            A->data[i][j] = Database->data[i][j] - mean->data[i][0];
        }
    }

    //printf("\ndeviation:\n");
    //matrix_print(A);

    //**************************************************************************
    //Calculate L, surrogate of covariance matrix, L = A'*A;
    //<.m: 42>
    L = matrix_constructor(P, P);

    cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, P, P, P, 1, *A->data, P, *A->data, P, 0, *L->data, P);

    printf("\nL = surrogate of covariance:\n");
    matrix_print(L);

    D = matrix_constructor(P, 1);

    LAPACKE_dsyev(LAPACK_ROW_MAJOR, 'V', 'U', P, *L->data, P, *D->data);
    V = L;
    L = NULL;

    //printf("D, eigenvalues:\n");
    //matrix_print(D);
    //printf("V, eigenvectors:\n");
    //matrix_print(V);

	//FREE INTERMEDIATES
    matrix_destructor(A);

    //...
    matrix_destructor(V);

    return M;
}

void DestroyFisher(MATRIX **M)
{
    matrix_destructor(M[0]);
//    matrix_destructor(M[1]);
//    matrix_destructor(M[2]);
//    matrix_destructor(M[3]);
    free(M);
}
