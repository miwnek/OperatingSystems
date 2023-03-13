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

runtests:
				make prepare_test
				./repl_static < command_file > results_static.txt
				./repl_shared < command_file > results_shared.txt
				./repl_dll    < command_file > results_dll.txt
				rm command_file

prepare_test:
				echo "init 2" > command_file

				echo "count library.c" >> command_file
				echo "count repl.c" >> command_file

				echo "show 0" >> command_file
				echo "show 1" >> command_file

				echo "delete 0" >> command_file
				echo "delete 1" >> command_file
				echo "destroy"  >> command_file
				echo "quit" >> command_file


clean:
				rm *.o *.a *so repl_shared repl_static repl_dll
