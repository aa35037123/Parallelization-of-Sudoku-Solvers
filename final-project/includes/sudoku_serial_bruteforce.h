#ifndef SUDOKUSERIALBRUTEFORCESOLVER_H
#define SUDOKUSERIALBRUTEFORCESOLVER_H

#include "sudoku.h"
#include "sudoku_solver.h"
#include <queue>

class SerialBruteForceSolver: public SudokuSolver {
public:
    SerialBruteForceSolver();
    SerialBruteForceSolver(const Sudoku& sudoku){
        init(sudoku);
    }

    void init(const Sudoku& sudoku) override;
    void solve() override;
    void display() const;

    bool is_valid(int row, int col, int num) const;
    bool find_empty(int &row, int &col) const;

private:
    struct State {
        Sudoku* sudoku;
        int row;  // current row in observation
        int col;  // current col in observation
    };
};

#endif
