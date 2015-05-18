# rshell
UCR Spring 2015

This is Jerome Barbero's version of rshell for Mike Izbicki's UCR Spring 2015 CS100 class.
It is a recreation of linux's bash terminal, where you should be able to read, write and execute files from and all that good computer science stuff.

## How To Run
1. git clone https://github.com/jbarb011/rshell.git
2. cd rshell
3. make
4. bin/rshell

## hw0 bugs:

1. Could not get || and && to work.

2. Too many spaces before executable cause segfaults.

3. Running too many programs at the same time causes slowdown.

4. If '#' is inside a word it will not count the comment.

5. If exit is not first parameter it will not exit program.

## hw1 bugs:

1. Could not get -R to work

2. --a works

3. a ls with both a file and folder will look weird

## hw2 features

(number)> and (number)>> both work

## hw2 bugs

1. Could not get piping to work

2. Sometimes after opening cat does not work for whatever reason. give it a couple of seconds.
