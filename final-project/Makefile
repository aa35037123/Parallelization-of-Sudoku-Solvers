# Makefile for Sudoku Solver

# Compiler
CXX = g++
MPICXX = mpic++

# Compiler flags

CXXFLAGS = -Wall -Wextra -std=c++17 -fopenmp -pthread
MPICXXFLAGS = -Wall -Wextra -std=c++17
# All output target executables
TARGETS = sudoku_main

DEPENDENCIES = \
	./src/sudoku.cpp \
	./src/sudoku_serial_bruteforce.cpp \
	./src/sudoku_parallel_bruteforce_pthread.cpp \
	./src/sudoku_parallel_bruteforce_omp.cpp \
	./src/sudoku_serial_backtracking.cpp \
	./src/sudoku_parallel_backtracking.cpp \
	./src/sudoku_parallel_backtracking_multiblocks.cpp \
	./src/sudoku_serial_genetic.cpp \
	./src/sudoku_parallel_dfs_grid.cpp \
	
all: $(TARGETS)

sudoku_main: sudoku_main.cpp $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -I ./includes -o $@ $^

clean:
	rm -f $(TARGETS) $(OBJECTS) solution.txt

.PHONY: all clean