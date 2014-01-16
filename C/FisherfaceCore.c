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

#include "ppm.h"
#include "CreateDatabase.h"
#include "FisherfaceCore.h"

MATRIX **FisherfaceCore(const database_t *D)
{
    int Class_population = 4; //Set value according to database (Images per person)
    int C = D->images / Class_population; //Number of classes (or persons)
    int P = D->images; //Total Number of training images
    int i, j, k;
    double temp = 0;
    MATRIX **M; //What the function returns
    MATRIX *mean; //Mean matrix
    MATRIX *A; //Deviation matrix
    MATRIX *L; //Surrogate of covariance matrix, L = A_trans * A

    M = (MATRIX **) malloc(4 * sizeof(MATRIX *));
    

    //**************************************************************************
    //Calculate mean
    //<.m: 36>
    mean = (MATRIX *) malloc(sizeof(MATRIX));
    mean->data = (double **) malloc (D->pixels * sizeof(double *));
    mean->rows = D->pixels;
    mean->cols = 1;

    for (i = 0; i < D->pixels; i++){
        mean->data[i] = (double *) malloc(sizeof(double));
    }

    //Calculate Mean matrix
    for (i = 0; i < D->pixels; i++) {
        temp = 0;
        for (j = 0; j < P; j++) {
            temp += (double) D->data[i][j].intensity;
        }
        mean->data[i][0] = (temp / P);
    }

    //Assign mean database
    M[0] = mean;

    //**************************************************************************
    //Calculate A, deviation matrix
    //<.m: 39>
    A = (MATRIX *) malloc(sizeof(MATRIX));
    A->data = (double **) malloc(D->pixels * sizeof(double *));
    A->rows = D->pixels;
    A->cols = P;
    for (i = 0; i < D->pixels; i++) {
        A->data[i] = (double *) malloc(P * sizeof(double));
        // each column in A->data is the difference between an image and the mean
        for (j = 0; j < P; j++) {
            A->data[i][j] = D->data[i][j].intensity - mean->data[i][0];
        }
    }
    
    //**************************************************************************
    //Calculate L, surrogate of covariance matrix
    //<.m: 42>
    L = (MATRIX *) malloc(sizeof(MATRIX));
    L->data = (double **) malloc(P * sizeof(double *));
    L->rows = P;
    L->cols = P;
    
    //1st loop controls row of L
    //2nd loop controls column of L
    //3rd loop iterates through a vector of pixels
    for (i = 0; i < P; i++) {
    	L->data[i] = (double *) malloc(P * sizeof(double));
    	for (j = 0; j < P; j++) {
        	temp = 0;
        	for (k = 0; k < D->pixels; k++) {
            	temp += A->data[k][i] * A->data[k][j];
        	}
        	L->data[i][j] = temp;
        }
        printf("Calculation %d\n", i);
    }    
    
	//FREE INTERMEDIATES
	for (i = 0; i < A->rows; i++) {
        free(A->data[i]);
    }
    free(A->data);
    free(A);
    for (i = 0; i < L->rows; i++) {
    	free(L->data[i]);
    }
    free(L->data);
    free(L);
	
    return M;
}

void DestroyFisher(MATRIX **M)
{
    int i;

    for(i = 0; i < M[0]->rows; i++){
        free(M[0]->data[i]);
    }
    free(M[0]->data);
    free(M[0]);
    //free(M[1]);
    //free(M[2]);
    //free(M[3]);
    free(M);
}
