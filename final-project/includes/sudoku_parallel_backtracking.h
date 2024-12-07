#ifndef SUDOKUPARALLELBACKTRACKINGSOLVER_H
#define SUDOKUPARALLELBACKTRACKINGSOLVER_H


#include "sudoku.h"
#include "sudoku_solver.h"

class ParallelBacktrackingSolver: public SudokuSolver{
public:
    ParallelBacktrackingSolver();
    ParallelBacktrackingSolver(const Sudoku& sudoku){
        init(sudoku);
    }

    void init(const Sudoku& sudoku) override;

    bool is_valid(int row, int col, int num, const Sudoku* local_result) const;

    void solve() override;

    void display() const;

private:
    bool find_empty(int &row, int &col, const Sudoku* local_result) const;
    bool backtracking(Sudoku* local_result);
    void copy_result(const Sudoku* local_result);
    void cleanup(Sudoku* local_result);
};

#endif