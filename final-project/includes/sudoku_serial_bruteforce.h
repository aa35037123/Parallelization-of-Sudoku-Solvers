#ifndef SUDOKUSERIALBRUTEFORCESOLVER_H
#define SUDOKUSERIALBRUTEFORCESOLVER_H


#include "sudoku.h"
#include "sudoku_solver.h"

class SerialBruteForceSolver: public SudokuSolver{
public:
    Sudoku* result;
    SerialBruteForceSolver() : result(nullptr) {}
    SerialBruteForceSolver(const Sudoku& sudoku) : result(nullptr) {
        init(sudoku);
    }

    void init(const Sudoku& sudoku) override;

    bool is_valid(int row, int col, int num) const;

    void solve() override;

    void display() const;

private:
    bool find_empty(int &row, int &col) const;
    bool bruteforce();
};

#endif