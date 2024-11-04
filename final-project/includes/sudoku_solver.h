#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H
#include "sudoku.h"

class SudokuSolver{
    public:
        virtual void init(const Sudoku& sudoku) = 0;
        virtual void solve() = 0;
};

#endif