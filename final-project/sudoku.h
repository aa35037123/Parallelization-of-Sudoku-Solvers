#ifndef SUDOKU_H
#define SUDOKU_H

#include <cstdint>
#include <string>

struct Sudoku {
    int size;
    uint8_t** grid;
};

uint8_t** allocateSudoku(int size);
void deallocateSudoku(Sudoku sudoku);
Sudoku loadSudoku(const std::string& filename);
void printSudoku(const Sudoku& sudoku);
bool isSudokuValid(const Sudoku& sudoku);

#endif
