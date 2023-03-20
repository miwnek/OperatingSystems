CC = gcc
CFLAGS = -Wall -g

all: libreverse.a
				$(CC) $(CFLAGS) -o reverse_1 reverse.c -L. libreverse.a -D BLOCK_LENGTH=1
				$(CC) $(CFLAGS) -o reverse_1024 reverse.c -L. libreverse.a -D BLOCK_LENGTH=1024

tests: libreverse.a
				$(CC) $(CFLAGS) -o tests tests.c -L. libreverse.a

libreverse.a:
				$(CC) $(CFLAGS) -c libreverse.c
				ar cr libreverse.a libreverse.o

clean:
		rm  reverse_1 reverse_1024 *.o *.a

clean_tests:
		rm tests *.o *.a
