example : example.o grayscale.o CreateDatabase.o ppm.o FisherfaceCore.o
	gcc -Wall -g example.o CreateDatabase.o grayscale.o ppm.o FisherfaceCore.o -o example

example.o : example.c CreateDatabase.c CreateDatabase.h ppm.h
	gcc -Wall -g -c example.c CreateDatabase.c

ppm.o : ppm.c ppm.h
	gcc -Wall -g -c ppm.c

CreateDatabase.o : CreateDatabase.c grayscale.c CreateDatabase.h ppm.h
	gcc -Wall -g -c CreateDatabase.c grayscale.c

grayscale.o : grayscale.c ppm.h
	gcc -Wall -g -c grayscale.c

FisherfaceCore.o : FisherfaceCore.c CreateDatabase.h ppm.h  FisherfaceCore.h
	gcc -Wall -g -c FisherfaceCore.c

clean:
	rm -rf *.o example core a.out
	clear