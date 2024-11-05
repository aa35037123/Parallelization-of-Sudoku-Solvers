#include "sudoku.h"

class SudokuSolver{
    public:
        virtual void init(const Sudoku& sudoku) = 0;
        virtual void solve() = 0;
}