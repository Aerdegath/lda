# Source: http://mrbook.org/tutorials/make/

CC=gcc
CFLAGS=-c -g -Wall
SOURCES=CreateDatabase.c example.c FisherfaceCore.c grayscale.c matrix.c ppm.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=example

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

matrix_unit: matrix_unit.o
	$(CC) $(CFLAGS) matrix_unit.o -o matrix_unit

matrix_unit.o: matrix_unit.c
	$(CC) $(CFLAGS) matrix_unit.c

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core
	clear
