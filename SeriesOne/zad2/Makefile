CC=gcc
CFLAGS=-Wall
LIB=library

# JUST FOR TESTING
all: tester

tester: main.o library.o
				$(CC) $(CFLAGS) -o tester main.o library.o

main.o: main.c
				$(CC) $(CFLAGS) -c main.c

library.o: library.c
				$(CC) $(CFLAGS) -c library.c
# END OF TESTING

static_lib: library.c
				$(CC) $(CFLAGS) -c library.c
				ar cr library.a library.o

shared_lib: library.c
				$(CC) $(CFLAGS) -c -fPIC library.c
				$(CC) $(CFLAGS) -shared -fPIC -o libshared.so library.o

#update later
clean:
		rm -f *.o *.a *.so tester
