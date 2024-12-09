#ifndef OMPSUDOKUPARALLELBRUTEFORCESOLVER_H
#define OMPSUDOKUPARALLELBRUTEFORCESOLVER_H

#include "sudoku.h"
#include "sudoku_solver.h"
#include "sudoku_serial_bruteforce.h"
#include <vector>
#include <iostream>

class SerialBruteforceSolverForParallel: public SerialBruteForceSolver{
    public:
        SerialBruteforceSolverForParallel(Sudoku sudoku) : SerialBruteForceSolver(sudoku) {};
        ~SerialBruteforceSolverForParallel() override = default;
        
        bool solve2();
        SerialBruteforceSolverForParallel(const SerialBruteforceSolverForParallel&) = delete;
        SerialBruteforceSolverForParallel& operator=(const SerialBruteforceSolverForParallel&) = delete;
        SerialBruteforceSolverForParallel(SerialBruteforceSolverForParallel&& other) noexcept 
            : SerialBruteForceSolver(std::move(other)) {
            result = other.result;
            other.result = nullptr;
        }
        SerialBruteforceSolverForParallel& operator=(SerialBruteforceSolverForParallel&&) noexcept;
};


class OMPParallelBruteForceSolver: public SudokuSolver {
public:
    OMPParallelBruteForceSolver() {
        result = nullptr;
    }
    OMPParallelBruteForceSolver(const Sudoku& sudoku){
        result = nullptr;
        init(sudoku);
    }
    ~OMPParallelBruteForceSolver() override = default;
    // ~OMPParallelBruteForceSolver() {
    //     if (result) {
    //         for (int i = 0; i < result->size; ++i) {
    //             delete[] result->grid[i];
    //         }
    //         delete[] result->grid;
    //         delete result;
    //     }
    // }
    void init(const Sudoku& sudoku) override;
    void solve() override;

private:
    bool is_valid(int row, int col, int num, const Sudoku* sudoku) const;
    void copy_result(const Sudoku& sudoku);
    bool find_empty(int &row, int &col, const Sudoku* local_result) const;
    std::vector<Sudoku*> init_unsolved_boards(const Sudoku* local_result);
    
private:
    int bootstrap = 4;
    std::vector<SerialBruteforceSolverForParallel*> unsolvedBoards;
};

#endif
