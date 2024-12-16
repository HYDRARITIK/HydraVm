# ihave 3 files main.c virtual.c and virtual.h in the same directory
# main.c is the main file
# virtual.c is the file that contains the functions
# virtual.h is the header file

# I have to create a makefile to compile the program

# Makefile

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
LDFLAGS = -lm

all: main

main: main.o virtual.o
	$(CC) $(CFLAGS) -o main main.o virtual.o $(LDFLAGS)

main.o: main.c virtual.h

virtual.o: virtual.c virtual.h

clean:
	rm -f main main.o virtual.o






