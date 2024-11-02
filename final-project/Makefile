# Makefile for Sudoku Solver

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Source files
TEST_SRCS = test.cpp sudoku.cpp

# Object files
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Executable name
TEST_TARGET = sudoku_test

.PHONY: test clean

# Rule to build the test executable
all:
test: $(TEST_TARGET)

# Rule to build the executable
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp sudoku.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TEST_OBJS) $(TEST_TARGET)
