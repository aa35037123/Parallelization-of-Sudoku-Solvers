# Makefile for Sudoku Solver

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# All output target executables
TARGETS = sudoku_main

DEPENDENCIES = \
	./src/sudoku.cpp \
	./src/sudoku_serial_bruteforce.cpp \
	./src/sudoku_serial_backtracking.cpp \
	./src/sudoku_parallel_backtracking.cpp \

all: $(TARGETS)

sudoku_main: sudoku_main.cpp $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) $(OPENMP) -I ./includes -o $@ $^

clean:
	rm -f $(TARGETS) $(OBJECTS) solution.txt