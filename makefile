CC=g++
FLAGS=-Wall -Werror -ansi

all:
	$(CC) $(FLAGS) main.cpp execute.cpp exitshell.cpp
