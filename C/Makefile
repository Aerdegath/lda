# Source: http://mrbook.org/tutorials/make/

CC=gcc

example: example.o CreateDatabase.o FisherfaceCore.o grayscale.o matrix.o ppm.o
	$(CC) -g -Wall example.o CreateDatabase.o FisherfaceCore.o -lblas -llapack matrix.o \
	ppm.o grayscale.o -o example

unit: matrix_unit.o matrix.o
	$(CC) -g -Wall matrix_unit.o matrix.o -o matrix_unit

matrix_unit.o: matrix_unit.c matrix.c matrix.h
	$(CC) -c -g -Wall matrix_unit.c

CreateDatabase.o: CreateDatabase.c CreateDatabase.h grayscale.h ppm.h
	$(CC) -c -g -Wall CreateDatabase.c

FisherfaceCore.o: FisherfaceCore.c FisherfaceCore.h ppm.h CreateDatabase.h matrix.h
	$(CC) -c -g -Wall FisherfaceCore.c

example.o: example.c CreateDatabase.h FisherfaceCore.h matrix.h ppm.h
	$(CC) -c -g -Wall example.c

grayscale.o: grayscale.c grayscale.h ppm.h
	$(CC) -c -g -Wall grayscale.c

matrix.o: matrix.c matrix.h
	$(CC) -c -g -Wall matrix.c

ppm.o: ppm.c ppm.h
	$(CC) -c -g -Wall ppm.c

clean:
	rm -rf *.o *.gch example matrix_unit
	clear
