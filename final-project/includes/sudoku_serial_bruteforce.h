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

    bool is_valid(int row, int col, int num, Sudoku& sudoku) const;
    bool find_empty(int &row, int &col, Sudoku& sudoku) const;

private:
    struct State {
        uint8_t num;
    };
};

#endif
