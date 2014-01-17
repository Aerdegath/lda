/******************************************************************************
  A sample script, which shows the usage of functions, included in
 FLD-based face recognition system (Fisherface method)

 See also: CREATEDATABASE, FISHERFACECORE, RECOGNITION

 Original version by Amir Hossein Omidvarnia, October 2007
                     Email: aomidvar@ece.ut.ac.ir
 ******************************************************************************/

#include <stdio.h>

#include "ppm.h"
#include "CreateDatabase.h"
#include "FisherfaceCore.h"

//These pathnames only work if working in the LDA/C folder
#define TrainDatabasePath "../LDAIMAGES/Train3"
#define TestDatabasePath "../LDAIMAGES/Test3"

int main(int argc, char *argv[])
{
    int load_stuff = 0; //Set to 0 if training database needs to be created
    //int pass = 0;
    //int fail = 0;
    
    database_t *D;
	MATRIX ** M;

    if (load_stuff == 0) {
		D = CreateDatabase(TrainDatabasePath);
		M = FisherfaceCore(D);

        // save to binary file
        // save output_faces.mat T m V_PCA V_Fisher ProjectedImages_Fisher;

		DestroyFisher(M);
		DestroyDatabase(D);

		return 0;
    } else {
    	fprintf(stderr, "Load the saved database");
        // read binary file
        // load output_faces.mat;
        return 1;
    }
}
