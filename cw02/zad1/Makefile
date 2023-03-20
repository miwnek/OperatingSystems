CC = gcc
CFLAGS = -Wall


all: libfiles.a
		$(CC) $(CFLAGS) -o swap_sys main.c -L. libfiles.a
		$(CC) $(CFLAGS) -o swap_lib main.c -L. libfiles.a -D SYS_FUNCS


tests: libfiles.a
		$(CC) $(CFLAGS) -o tests tests.c -L. libfiles.a

libfiles.a:
				$(CC) $(CFLAGS) -c libfiles.c
				ar cr libfiles.a libfiles.o

clean:
		rm  swap_sys swap_lib *.o *.a

clean_tests:
		rm tests *.o *.a
