CC=gcc
CFLAGS=-Wall 
LIB=library

all:
				make static
				make shared
				make repl_static
				make repl_shared
				make repl_dll

static: library.c
				$(CC) $(CFLAGS) -c library.c
				ar cr library.a library.o

shared: library.c
				$(CC) $(CFLAGS) -c -fPIC library.c
				$(CC) $(CFLAGS) -shared -o library.so library.o

repl_static: repl.c library.c dllhandle.h
				$(CC) $(CFLAGS) -o repl_static repl.c -L. library.a

repl_shared: repl.c library.c dllhandle.h
				$(CC) $(CFLAGS) -L. -Wl,-rpath=. -o repl_shared repl.c library.so

repl_dll: repl.c dllhandle.h library.c
				$(CC) $(CFLAGS) -L. -Wl,-rpath=. -o repl_dll repl.c -ldl -D LIBTYPE_DLL

clean:
				rm *.o *.a *so repl_shared repl_static repl_dll
