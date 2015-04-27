all:
	make bin;
	make rshell;
	make ls;
bin:
	mkdir bin;

rshell:
	src/rshell.cpp -Wall -Werror -ansi -pedantic -o bin/rshell

ls:
	src/ls.cpp -Wall -Werror -ansi -pedantic -o bin/ls

