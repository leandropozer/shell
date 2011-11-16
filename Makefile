shell : builtin.o main.o list.o parser.o
	gcc obj/builtin.o obj/main.o obj/list.o obj/parser.o -o bin/shell

builtin.o : builtin.c
	gcc -c builtin.c -o obj/builtin.o

main.o : main.c
	gcc -c main.c -o obj/main.o

list.o : list.c
	gcc -c list.c -o obj/list.o

parser.o : parser.c
	gcc -c parser.c -o obj/parser.o
