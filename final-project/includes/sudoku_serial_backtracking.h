#ifndef SUDOKUSERIALBACKTRACKINGSOLVER_H
#define SUDOKUSERIALBACKTRACKINGSOLVER_H

#include "sudoku.h"
#include "sudoku_solver.h"

class SerialBacktrackingSolver: public SudokuSolver{
public:
    SerialBacktrackingSolver();
    SerialBacktrackingSolver(const Sudoku& sudoku) {
        init(sudoku);
    }

    void init(const Sudoku& sudoku) override;

    bool is_valid(int row, int col, int num) const;

    void solve() override;

    void display() const;

private:
    bool find_empty(int &row, int &col) const;
    bool backtracking();
};
#endif