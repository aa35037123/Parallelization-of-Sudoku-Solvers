#ifndef SUDOKUPARALLELBACKTRACKINGMULTIBLOCKSSOLVER_H
#define SUDOKUPARALLELBACKTRACKINGMULTIBLOCKSSOLVER_H

#include "sudoku.h"
#include "sudoku_solver.h"
#include <deque>
#include <omp.h>

class ParallelBacktrackingMultiBlocksSolver : public SudokuSolver {
public:
    ParallelBacktrackingMultiBlocksSolver();
    ParallelBacktrackingMultiBlocksSolver(const Sudoku& sudoku) {
        init(sudoku);
    }

    void init(const Sudoku& sudoku) override;

    void solve() override;

    void display() const;

private:
    bool is_valid(const Sudoku* local_result, int row, int col, int num);
    bool find_empty(const Sudoku* local_result, int &row, int &col) const;
    void bootstrap(const int iterations = 5);
    void parallel_backtracking();
    void copy_result(const Sudoku* local_result);
    void cleanup(Sudoku* local_result);

    struct State {
        Sudoku* board;
        int row;
        int col;
    };

    std::deque<State> board_queue; // Shared deque for potential boards
    omp_lock_t queue_lock;         // Lock to protect access to the deque
    bool solved;                   // Shared flag for early exit
};

#endif
