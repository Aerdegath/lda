/****************************************
 Recognizing step....

 Description: This function compares two faces by projecting the images into
 facespace and measuring the Euclidean distance between them.

 Argument:      TestImage              - Path of the input test image

                m_database             - (M*Nx1) Mean of the training database
                                         database, which is output of
                                         'EigenfaceCore' function.

                V_PCA                  - (M*Nx(P-1)) Eigen vectors of the
                                         covariance matrix of
                                         the training database

                V_Fisher               - ((P-1)x(C-1)) Largest (C-1) eigen
                                         vectors of matrix J = inv(Sw) * Sb

                ProjectedImages_Fisher - ((C-1)xP) Training images, which
                                         are projected onto Fisher linear space

 Returns:       OutputName             - Name of the recognized image in the
                                         training database.

 See also: RESHAPE, STRCAT

 Original version by Amir Hossein Omidvarnia, October 2007
                     Email: aomidvar@ece.ut.ac.ir
 ****************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ppm.h"

typedef struct {
    double ** data;
    int rows, cols;
} MATRIX;

MATRIX m_database; //ends up being a 1d matrix
MATRIX Inverse_V_Fisher;
MATRIX Inverse_V_PCA;
MATRIX ProjectedImages_Fisher;
MATRIX Difference;
MATRIX v_fisherT_x_v_pcaT;
MATRIX ProjectedTestImage;

int MatrixRead_Binary(); // This version reads using fread() assumption:
// matrix was written binary

int main()
{
    char filename[255] = {}; // filename of the test image

    // We need to load the data structures that were created by the
    // CreateDatabase step
    if (!MatrixRead_Binary()) { //Load structures
        printf("Error!!!\n");
        exit(-1);
    }
    printf("Database Files Loaded From Disk...\n");
    PPMImage *TestImage; // pointer to the structure of our loaded test image

    int pass = 0, fail = 0, iterations; // Let's keep a few stats
    int i, j, k, x; // loop variables

    for (iterations = 1; iterations <= 30; iterations++) {
        /* 994 is the number of test images that we have sequentially numbered */
        // concat our filename together
        sprintf(filename, "../LDAIMAGES/Test2/%d.ppm", iterations);
        TestImage = ppm_image_constructor(filename);
        printf("Test Image Loaded From Disk and converted to grayscale...\n");

        // First let's allocate our difference array
        Difference.rows = m_database.rows;
        Difference.cols = 1;
        // Difference.data=new double*[Difference.rows];
        Difference.data = (double**) malloc(Difference.rows * sizeof (double*));
        for (i = 0; i < Difference.rows; i++) {
            // Difference.data[i]=new double[Difference.cols];
            Difference.data[i] = (double*) malloc(Difference.cols
                    * sizeof (double));
        }

        for (i = 0; i < m_database.rows; i++) {
            Difference.data[i][0] = TestImage->pixels[i].r
                    - m_database.data[i][0]; // mean database is a 1d vector
            // (using red channel for gray)
        }
        // Now let's multiply in the last matrix to calculate our ProjectedTestImage
        //v_fisherT_x_v_pcaT * Difference
        // Why is this commented out?

        ProjectedTestImage.rows = v_fisherT_x_v_pcaT.rows;
        ProjectedTestImage.cols = Difference.cols;
        ProjectedTestImage.data = (double**) malloc(ProjectedTestImage.rows
                * sizeof (double*));
        // allocate memory for our new array
        for (i = 0; i < ProjectedTestImage.rows; i++) {
            ProjectedTestImage.data[i] = (double*) malloc(ProjectedTestImage.cols
                    * sizeof (double));
            if (ProjectedTestImage.data[i] == 0) {
                printf("Dynamic Allocation Failed!!!\n");
                return -1;
            }
        }

        for (i = 0; i < v_fisherT_x_v_pcaT.rows; i++) { // perform matrix mult.
            // computation
            for (j = 0; j < Difference.cols; j++) { // This loop executes once
                ProjectedTestImage.data[i][j] = 0.0;
                for (k = 0; k < v_fisherT_x_v_pcaT.cols; k++) {
                    ProjectedTestImage.data[i][j] +=
                            v_fisherT_x_v_pcaT.data[i][k] * Difference.data[k][j];
                }
            }
        }

        unsigned long int Train_Number = 0;
        Train_Number = ProjectedImages_Fisher.cols; // Satisfies line 27
        double * q = 0; // Holds a column vector
        q = (double *) malloc(ProjectedImages_Fisher.rows * sizeof (double));

        double * Euc_dist = (double *)
                malloc(sizeof (double) * ProjectedImages_Fisher.cols);
        double temp = 0;

        for (i = 0; i < Train_Number; i++) { // line 44 Recognition.m
            for (j = 0; j < ProjectedImages_Fisher.rows; j++) { // create q
                q[j] = ProjectedImages_Fisher.data[j][i];
            } //q has been populated

            // At this point, ProjectedTestImage is 99x1 and q is 99x1
                    // (Based on testing database)
            for (x = 0; x < ProjectedImages_Fisher.rows; x++) {
                temp += ((ProjectedTestImage.data[x][0] - q[x]) *
                        (ProjectedTestImage.data[x][0] - q[x])); //line 46
            }
            Euc_dist[i] = temp;
            temp = 0; //reset our running count
        }
        // at this point, Euc_dist should be populated

        // we need to find the min euc_dist and its index
        //int Euc_dist_len = sizeof(*Euc_dist) / sizeof(double);
        int Euc_dist_len = ProjectedImages_Fisher.cols; // the length of euc_dist
        double min = Euc_dist[0];

        int Recognized_index = 0;
        for (i = 0; i < Euc_dist_len; i++) {
            if (Euc_dist[i] < min) { // reassign min
                min = Euc_dist[i];
                Recognized_index = i;
            }
        }
        int filename_index = Recognized_index + 1; // because our files are
        // named starting at 1 and not 0. Arrays start at 0 in C

        printf("Test %d : %d.ppm == %d.ppm\n", iterations, iterations,
                filename_index);
        printf("-----------------------------------\n");

        ////////////// for statistic tracking
        if (iterations == ((Recognized_index - 1) / 4 + 1)) {
            pass++;
        } else {
            fail++;
        }
        //////////////
        ppm_image_destructor(TestImage, 0); // Free the loaded image
        // Need to free the memory that was allocated...

        // free Difference matrix
        for (i = 0; i < Difference.rows; i++) {
            free(Difference.data[i]);
        }
        free(Difference.data);

        // Free ProjectedTestImage matrix...
        for (i = 0; i < ProjectedTestImage.rows; i++) {
            free(ProjectedTestImage.data[i]);
        }
        free(ProjectedTestImage.data);

        // Free q vector
        free(q); // wha? why exactly did i use malloc?

        // Free Euc_dist
        free(Euc_dist); // huh?
        ///////////Allocated Memory Freed//////////////////////
    }
    printf("%d Correct %d Wrong\n", pass, fail);
    return 0;
}

/*MatrixRead_Binary() returns 0 on error and 1 on success*/
int MatrixRead_Binary() //Reads in all required matrices
{
    int rows, cols;
    int i, j;
    FILE *fin = 0;
    /**********************Read In The m.mat*************************/
    fin = fopen("m.mat", "rb");
    if (fin == NULL) {
        printf("Unable to Open m.mat!!!\n");
        return (0);
    }
    fread(&rows, sizeof (int), 1, fin);
    fread(&cols, sizeof (int), 1, fin);
    printf("m.mat [%d %d]\n", rows, cols);
    m_database.data = (double**) malloc(rows * sizeof (double*));
    for (i = 0; i < rows; i++) {
        m_database.data[i] = (double*) malloc(cols * sizeof (double));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fread(&(m_database.data[i][j]), sizeof (double), 1, fin);
            /*printf("data verify (%lf)\n", m_database.data[i][j]);
            while(1) {
                if(getchar() == 'n') break;
            }*/
        }
    }
    printf("read m.mat!!!\n");
    m_database.rows = rows;
    m_database.cols = cols;
    //fin.close();
    fclose(fin);
    fin = 0;
    /**************************************************************/

    /**************Read In The Inverse_V_Fisher.mat****************/
    /*fin.open("Inverse_V_Fisher.mat",ios::in|ios::binary);
    if (!fin.is_open()) {
        cerr << "Unable to Open Inverse_V_Fisher.mat!!!" << endl;
        return 0;
    }
    fin.read((char*)&rows, sizeof(rows));
    fin.read((char*)&cols, sizeof(cols));*/
    fin = fopen("Inverse_V_Fisher.mat", "rb");
    if (fin == NULL) {
        printf("Unable to Open Inverse_V_Fisher.mat!!!\n");
        return (0);
    }
    fread(&rows, sizeof (int), 1, fin);
    fread(&cols, sizeof (int), 1, fin);
    printf("Inverse_V_Fisher.mat [%d %d]\n", rows, cols);
    //Inverse_V_Fisher.data=new double*[rows];
    Inverse_V_Fisher.data = (double**) malloc(rows * sizeof (double*));
    for (i = 0; i < rows; i++) {
        //Inverse_V_Fisher.data[i]=new double[cols];
        Inverse_V_Fisher.data[i] = (double*) malloc(cols * sizeof (double));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //fin.read((char*)&Inverse_V_Fisher.data[i][j],
                    //sizeof(Inverse_V_Fisher.data[i][j]));
            fread(&(Inverse_V_Fisher.data[i][j]), sizeof (double), 1, fin);
            /*printf("data verify (%.20lf)\n", Inverse_V_Fisher.data[i][j]);
            while(1)
            {
                if(getchar() == 'n') break;
            }*/
        }
    }
    printf("read Inverse_V_Fisher.mat!!!\n");
    Inverse_V_Fisher.rows = rows;
    Inverse_V_Fisher.cols = cols;
    //fin.close();
    fclose(fin);
    fin = 0;
    /**************************************************************/


    /**************Read In The Inverse_V_PCA.mat****************/
    /*fin.open("Inverse_V_PCA.mat",ios::in|ios::binary);
    if(!fin.is_open())
        {
            cerr << "Unable to Open Inverse_V_PCA!!!" << endl;
            return 0;
        }
    fin.read((char*)&rows,sizeof(rows));
    fin.read((char*)&cols,sizeof(cols));*/
    fin = fopen("Inverse_V_PCA.mat", "rb");
    if (fin == NULL) {
        printf("Unable to Open Inverse_V_PCA!!!\n");
        return (0);
    }
    fread(&rows, sizeof (int), 1, fin);
    fread(&cols, sizeof (int), 1, fin);
    printf("Inverse_V_PCA.mat [%d %d]\n", rows, cols);
    //Inverse_V_PCA.data=new double*[rows];
    Inverse_V_PCA.data = (double**) malloc(rows * sizeof (double*));
    for (i = 0; i < rows; i++) {
        //Inverse_V_PCA.data[i]=new double[cols];
        Inverse_V_PCA.data[i] = (double*) malloc(cols * sizeof (double));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //fin.read((char*)&Inverse_V_PCA.data[i][j],
                    //sizeof(Inverse_V_PCA.data[i][j]));
            fread(&(Inverse_V_PCA.data[i][j]), sizeof (double), 1, fin);
            /*printf("data verify (%.20lf) total read - %d\n",
                    Inverse_V_PCA.data[i][j], ((i*cols)+j+1));
            while (1) {
                if(getchar() == '\n') break;
            }*/
        }
    }
    printf("read Inverse_V_PCA.mat!!!\n");
    Inverse_V_PCA.rows = rows;
    Inverse_V_PCA.cols = cols;
    //fin.close();
    fclose(fin);
    fin = 0;
    /**************************************************************/

    /**************Read In The ProjectedImages_Fisher.mat****************/
    /*fin.open("ProjectedImages_Fisher.mat",ios::in|ios::binary);
    if(!fin.is_open())
        {
            cerr << "Unable to Open ProjectedImages_Fisher.mat!!!" << endl;
            return 0;
        }
    fin.read((char*)&rows,sizeof(rows));
    fin.read((char*)&cols,sizeof(cols));*/
    fin = fopen("ProjectedImages_Fisher.mat", "rb");
    if (fin == NULL) {
        printf("Unable to Open ProjectedImages_Fisher.mat!!!\n");
        return (0);
    }
    fread(&rows, sizeof (int), 1, fin);
    fread(&cols, sizeof (int), 1, fin);
    printf("ProjectedImages_Fisher.mat [%d %d]\n", rows, cols);
    //ProjectedImages_Fisher.data=new double*[rows];
    ProjectedImages_Fisher.data = (double**) malloc(rows * sizeof (double*));
    for (i = 0; i < rows; i++) {
        //ProjectedImages_Fisher.data[i]=new double[cols];
        ProjectedImages_Fisher.data[i] = (double*) malloc(cols * sizeof (double));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //fin.read((char*)&ProjectedImages_Fisher.data[i][j],sizeof(double));
            fread(&(ProjectedImages_Fisher.data[i][j]), sizeof (double), 1, fin);
        }
    }
    printf("read ProjectedImages_Fisher.mat!!!\n");
    fflush(stdout);
    ProjectedImages_Fisher.rows = rows;
    ProjectedImages_Fisher.cols = cols;
    //fin.close();
    fclose(fin);

    /**************************************************************/

    /**************Read In The v_fisherT_x_v_pcaT.mat****************/
    /*fin.open("v_fisherT_x_v_pcaT.mat",ios::in|ios::binary);
    if(!fin.is_open())
        {
            cerr << "Unable to Open v_fisherT_x_v_pcaT.mat!!!" << endl;
            return 0;
        }
    fin.read((char*)&rows,sizeof(rows));
    fin.read((char*)&cols,sizeof(cols));*/
    fin = fopen("v_fisherT_x_v_pcaT.mat", "rb");
    if (fin == NULL) {
        printf("Unable to Open v_fisherT_x_v_pcaT.mat!!!\n");
        return (0);
    }
    fread(&rows, sizeof (int), 1, fin);
    fread(&cols, sizeof (int), 1, fin);
    printf("v_fisherT_x_v_pcaT.mat [%d %d]\n", rows, cols);
    //v_fisherT_x_v_pcaT.data=new double*[rows];
    v_fisherT_x_v_pcaT.data = (double**) malloc(rows * sizeof (double*));
    for (i = 0; i < rows; i++) {
        //v_fisherT_x_v_pcaT.data[i]=new double[cols];
        v_fisherT_x_v_pcaT.data[i] = (double*) malloc(cols * sizeof (double));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //fin.read((char*)&v_fisherT_x_v_pcaT.data[i][j],
                    //sizeof(v_fisherT_x_v_pcaT.data[i][j]));
            fread(&(v_fisherT_x_v_pcaT.data[i][j]), sizeof (double), 1, fin);

        }
    }
    printf("read v_fisherT_x_v_pcaT.mat!!!\n");
    fflush(stdout);
    v_fisherT_x_v_pcaT.rows = rows;
    v_fisherT_x_v_pcaT.cols = cols;
    //fin.close();
    fclose(fin);
    /**************************************************************/

    //Now make sure everything is opened properly
    if (m_database.data == 0 || Inverse_V_Fisher.data == 0
            || Inverse_V_PCA.data == 0 || ProjectedImages_Fisher.data == 0
            || v_fisherT_x_v_pcaT.data == 0) {
        return 0; //Memory not allocated properly somewhere
    } else
        return 1;
}