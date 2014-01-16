/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
   MA 02110-1301 USA
 */
 
//GLOBAL VARIABLES FOR WIDTH AND HEIGHT
//These must be changed to the correct width/height of ppm files being used
#ifndef WIDTH
#define WIDTH 128
#endif
#ifndef HEIGHT
#define HEIGHT 192
#endif

typedef struct {
    double ** data;
    int rows, cols;
} MATRIX;

// creates the database from files in the directory
// calls database_constructor
database_t *CreateDatabase(char TrainPath[]);

// actual constructor
database_t *database_constructor(int pixels, int images);

//Destructor
void DestroyDatabase(database_t *D);