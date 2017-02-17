CC=g++
FLAGS=-Wall -Werror -ansi -pedantic
EXEC=a.out

all:
	mkdir -p bin
	$(CC) $(FLAGS) ./src/main.cpp ./src/execute.cpp ./src/exitshell.cpp ./src/connector.cpp -o ./bin/$(EXEC)
rshell:
	mkdir -p bin
	$(CC) $(FLAGS) ./src/main.cpp ./src/execute.cpp ./src/exitshell.cpp ./src/connector.cpp -o ./bin/$(EXEC)
clean:
	rm -r bin