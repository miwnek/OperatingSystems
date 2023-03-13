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
				$(CC) $(CFLAGS) $(OFLAG) -c library.c
				ar cr library.a library.o

shared: library.c
				$(CC) $(CFLAGS) $(OFLAG) -c -fPIC library.c
				$(CC) $(CFLAGS) $(OFLAG) -shared -o library.so library.o

repl_static: repl.c library.c dllhandle.h
				$(CC) $(CFLAGS) $(OFLAG) -o repl_static repl.c -L. library.a

repl_shared: repl.c library.c dllhandle.h
				$(CC) $(CFLAGS) $(OFLAG) -L. -Wl,-rpath=. -o repl_shared repl.c library.so

repl_dll: repl.c dllhandle.h library.c
				$(CC) $(CFLAGS) $(OFLAG) -L. -Wl,-rpath=. -o repl_dll repl.c -ldl -D LIBTYPE_DLL

runtests:
				make prepare_test
				./repl_static < command_file > results_static$(OTAG).txt
				./repl_shared < command_file > results_shared$(OTAG).txt
				./repl_dll    < command_file > results_dll$(OTAG).txt
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

O0:
				make all OFLAG="-O0"
				make runtests OTAG="_O0"

Os:
				make all OFLAG="-Os"
				make runtests OTAG="_Os"

O3:
				make all OFLAG="-O3"
				make runtests OTAG="_O3"


clean:
				rm *.o *.a *so repl_shared repl_static repl_dll
