all: memgrind.c mymalloc.o
	gcc memgrind.c mymalloc.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c

clean: 
	rm mymalloc.o memgrind