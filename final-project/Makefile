# Makefile for Sudoku Solver

# Compiler
CXX = g++

# Compiler flags

CXXFLAGS = -Wall -Wextra -std=c++17 -fopenmp

# All output target executables
TARGETS = sudoku_main

DEPENDENCIES = \
	./src/sudoku.cpp \
	./src/sudoku_serial_bruteforce.cpp \
	./src/sudoku_parallel_bruteforce_omp.cpp \
	./src/sudoku_serial_backtracking.cpp \
	./src/sudoku_parallel_backtracking.cpp \
	./src/sudoku_parallel_backtracking_multiblocks.cpp \
	./src/sudoku_serial_genetic.cpp \
	


all: $(TARGETS)

sudoku_main: sudoku_main.cpp $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -I ./includes -o $@ $^

clean:
	rm -f $(TARGETS) $(OBJECTS) solution.txt

.PHONY: all clean