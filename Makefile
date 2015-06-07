all: bin rshell ls cp rm mv

bin: bin
	mkdir bin;

rshell: bin
	g++ -g src/shell.cpp -Wall -Werror -ansi -pedantic -o bin/rshell

ls: bin
	g++ -g src/ls.cpp -Wall -Werror -ansi -pedantic -o bin/ls

cp: bin
	g++ src/cp.cpp -Wall -Werror -ansi -pedantic -o bin/cp

rm: bin
	g++ src/rm.cpp -Wall -Werror -ansi -pedantic -o bin/rm

mv: bin
	g++ src/mv.cpp -Wall -Werror -ansi -pedantic -o bin/mv
