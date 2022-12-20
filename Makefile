# CS4280 11/16/22 Bryce Paubel P4
# Makefile to compile P4

# NOTE THAT THIS IS HEAVILY BASED ON A MAKEFILE
# FROM CS2750, WHICH I TOOK SUMMER 2021 AND BASED MY
# MAKEFILE HEAVILY ON AN EXAMPLE WITHIN THE COURSE

CC = g++

FLAGS = -Wall -g -std=c++11

DEPS = ./src/main.cpp ./src/files.cpp ./src/scanner.cpp ./src/token.cpp ./src/parser.cpp ./src/ptree.cpp ./src/semanticAnalyzer.cpp ./src/semanticStack.cpp ./src/codeGen.cpp ./src/compiler.cpp ./src/globalTable.cpp
OBJ = ./build/main.o ./build/files.o ./build/scanner.o ./build/token.o ./build/parser.o ./build/ptree.o ./build/semanticAnalyzer.o ./build/semanticStack.o ./build/codeGen.o ./build/compiler.o ./build/globalTable.o

TARGET = compfs

# By default make will start with the first target
# which in this case is our executable, target
$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $@ $^
	rm $(OBJ)

./build/%.o: ./src/%.cpp $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $<

./build/%.o: ./test/%.cpp $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $<

# Run the program (if not compiled, will compile)
# This works with default input, from the keyboard
.PHONY: run
run:
	make
	$(TARGET)