CC=gcc
CFLAGS=-Wall 
LIB=library

all:
				make static_lib
				# make shared_lib
				make static_runnable

static_lib: library.c
				$(CC) $(CFLAGS) -c library.c
				ar cr library.a library.o

shared_lib: library.c
				$(CC) $(CFLAGS) -c -fPIC library.c
				$(CC) -shared -o library.so library.o

static_runnable:
				$(CC) $(CFLAGS) -c repl.c
				$(CC) -o repl repl.o -L. library.a

clean:
				rm *.o *.a repl
