CC=gcc
CFLAGS=-Wall 
LIB=library

all:
				make static_lib
				make shared_lib

static_lib: library.c
				$(CC) $(CFLAGS) -c library.c
				ar cr library.a library.o

shared_lib: library.c
				$(CC) $(CFLAGS) -c -fPIC library.c
				$(CC) -shared -o library.so library.o

clean:
		rm -f *.o *.a *.so 
