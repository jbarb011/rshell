all:
	make bin;
	make rshell;
	make ls;
bin:
	mkdir bin;

rshell:
	g++ rc/rshell.cpp -Wall -Werror -ansi -pedantic -o bin/rshell

ls:
	g++ src/ls.cpp -Wall -Werror -ansi -pedantic -o bin/ls

