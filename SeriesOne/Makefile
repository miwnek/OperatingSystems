all: tester

tester: main.o library.o
			 gcc main.o library.o -o tester

main.o: main.c
				gcc -c main.c

library.o: library.c
						gcc -c library.c

clean:
		rm *o tester
