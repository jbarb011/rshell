all:
	make bin;
	make rshell;

bin:
	mkdir bin;

rshell:
	g++ src/shell.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
