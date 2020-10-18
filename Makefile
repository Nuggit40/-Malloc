all: memgrind.c mymalloc.o
	gcc memgrind.c mymalloc.o
mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c