/*******************************************************************************
Align a set of face images (the training set T1, T2, ... , TM)

Description: This function reshapes all 2D images of the training database into
column vectors. Then, it puts these 1D column vectors in a row to construct 2D
matrix 'T'. Each column of 'T' is a training image, which has been reshaped into
a vector.
P: the total number of MxN training images.
C: the number of classes.

Argument:     TrainDatabasePath     - Path of the training database

Returns:      T                     - A 2D matrix, containing all 1D image
                                      vectors.
                                      The length of 1D column vectors is MN
                                      and 'T' will be a MNxP 2D matrix.

See also: STRCMP, STRCAT, RESHAPE

Original version by Amir Hossein Omidvarnia, October 2007
                 Email: aomidvar@ece.ut.ac.ir
******************************************************************************/

/**
Note that at present the way that the files are read in is not in sequential
order by filename (i.e., "natural ordering"). Instead all files with same
starting number are read in together. (e.g., 1, 10, 11, 12, ..., 2, 20, 21, 22,
...) Perhaps sort the file list sequentially or guess the filenames
sequentially.
 **/

#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
#include "CreateDatabase.h"

/* All training images should have the following extension otherwise they are
   skipped */
#define EXTENSION ".ppm"

//Global variable: number of image files
int ImageCount;

// Arguments: Path to Directory of Training Images
// Returns: NULL on error
database_t *CreateDatabase(char TrainPath[])
{
    PPMImage *image; // temp pointer used in image conversion to 1D
    Pixel ** T; // Return 2D matrix of column vectors; each column is a linearized image
    Pixel *Tp; // used for allocating the memory used in T
    Pixel *pix_ptr; // pointer to a pixel
    database_t *final;

    int i = 0;
    int j = 0;
    DIR *dir = opendir(TrainPath); // Open Directory Pointer
    struct dirent *myDirEntry; // pointer to directory entry
    char *FullPath; // path of file, e.g., ../LDAIMAGES/Train2/1.ppm
    int FileCount = 0;
    char **Files; // list of filenames
    if (!dir) { // Failed to open directory
        return NULL;
    }

    // number of entries in the directory
    myDirEntry = readdir(dir);
    while (myDirEntry) {
        if (strstr(myDirEntry->d_name, EXTENSION)) {
            ImageCount++; // number of actual image files
        }
        ++FileCount;
         myDirEntry = readdir(dir);
    }

    // Now let's go back and extract all filenames
    rewinddir(dir); // move dir pointer back to beginning

    // Allocate memory for list of names
    Files = (char **) malloc(ImageCount * sizeof(char *));

    if (!Files) { //Dynamic Memory Allocation Failed
        return NULL;
    }

    // Read in the names of each file
    for (i = 0; i < ImageCount; i++) {
        myDirEntry = readdir(dir);

        // skip files that don't match "*.ppm"
        while (!strstr(myDirEntry->d_name, EXTENSION)) {
            myDirEntry = readdir(dir);
        }

        // copy the filename into listing; takes care of allocation
        Files[i] = strdup(myDirEntry->d_name);
        if (!Files[i]) { // i.e., problem allocating
        // free up what was allocated so far (0 up to i) and get out of here
            for (j = i; j >= 0; j--) {
                free(Files[i]);
            }
            return NULL;
        }
    }

    //////////////Create Database Here///////////////
    FullPath = (char *) malloc (255 + strlen(TrainPath) + 2);

    //printf("# files = %d; # images = %d\n", FileCount, ImageCount);

    // allocate a row for each pixel
    // changed this for use with LAPACK; still need to debug
    T = (Pixel **) malloc (WIDTH * HEIGHT * sizeof(Pixel *));
    Tp = (Pixel *) malloc ((WIDTH * HEIGHT) * ImageCount * sizeof(Pixel)); // ensures memory is contiguous

    // allocate a column for each image
    for(i = 0; i < WIDTH * HEIGHT; i++){
        T[i] = &Tp[i * ImageCount];
    }

    // for each image (each image being a column of T)
    for (j = 0; j < ImageCount; j++) {
        sprintf(FullPath, "%s/%s", TrainPath, Files[j]);
        // FullPath is now the entire path to image in question

        image = ppm_image_constructor(FullPath);
        grayscale(image); // convert image to grayscale in-place

        pix_ptr = image->pixels;
        // for each row (each pixel being a row of T)
        for (i = 0; i < WIDTH * HEIGHT; i++) {
            T[i][j] = *pix_ptr; // copy pixel intensity data
            pix_ptr++;
        }

        ppm_image_destructor(image, 1);
    }

    free(FullPath);
    /////////////////////////////////////////////////

    /* Once all files have been loaded and database created we need to free
    the memory that was used to store the list and the filenames */
    for (i = 0; i < ImageCount; i++) {
       free(Files[i]);
    }
    free(Files);
    Files = NULL;
    free(dir);
    dir = NULL;

    //COMMENTED OUT AT THE MOMENT BECAUSE OF MEMORY REASONS
    //Save final data into database_t struct
    //final = database_constructor(WIDTH * HEIGHT, ImageCount);

    final = malloc(sizeof(database_t));
    final->data = T;
    final->images = ImageCount;
    final->pixels = WIDTH * HEIGHT;

    return final;
}

/*
 * Constructs the database_t object
 * pixels: total number of pixels in the database (M*N*ImageCount)
 * images
 */
database_t *database_constructor(const int pixels, const int images)
{
    int i;
    database_t * database;
    Pixel ** data;

    database = malloc(sizeof(database_t));

    // allocate data
    data = (Pixel **) malloc(pixels * sizeof(Pixel *));
    for (i = 0; i < pixels; i++) {
        data[i] = (Pixel *) malloc(images * sizeof(Pixel));
    }

    database->data = data;
    database->pixels = pixels;
    database->images = images;

    return database;
}

/*
 * Frees the database_t object
 * D: the database to be freed
 */
void DestroyDatabase(database_t *D)
{
    int i = 0;

    for(i = 0; i < D->pixels; i++) {
        free(D->data[i]);
    }
    free(D->data);
    free(D);
}
