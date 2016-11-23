all: bin/functions.o
	gcc -fPIC -shared -Wl,-soname,functions.so -o functions.so bin/functions.o

bin/functions.o:
	gcc -c -Iinclude/ -fPIC -Wall -o bin/functions.o src/functions.c
