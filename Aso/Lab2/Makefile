all : main

fun1.o : fun1.c
	gcc -c fun1.c -o fun1.o

fun2.o : fun2.c
	gcc -c fun2.c -o fun2.o

libprueba.a : fun1.o fun2.o
	ar cr libprueba.a fun1.o fun2.o

main.o : main.c
	gcc -c main.c -o main.o

main : main.o libprueba.a
	gcc -o main main.o -L. -lprueba

clean : 
	rm -f *.o *.a main