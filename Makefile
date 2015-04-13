all:
	make bin;
	make rshell;

bin:
	mkdir bin;

rshell:
	src/rshell.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
