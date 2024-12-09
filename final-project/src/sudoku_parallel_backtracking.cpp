#include <iostream>
#include <cmath>
#include <omp.h>
#include "sudoku_parallel_backtracking.h"
#include "sudoku_serial_backtracking.h"
#define NUM_THREADS 4


// extern Sudoku* result;

bool SerialBacktrackingSolverForParallel::solve2() {
    return backtracking();
}

bool SerialBacktrackingSolverForParallel::backtracking() {
    if (*solved) {
        return true;
    }

    int row, col;

    // Find the next empty cell
    if (!find_empty(row, col)) {
        *solved = true;
        return true;  // No empty cells left, puzzle is solved
    }

    for (int num = 1; num <= result->size; ++num) {
        if (is_valid(row, col, num)) {
            result->grid[row][col] = num;

            if (backtracking()) {
                return true;
            }

            // Backtrack
            result->grid[row][col] = 0;
        }
    }
    return false;
}


void ParallelBacktrackingSolver::init(const Sudoku& sudoku) {
    omp_set_dynamic(0);
    omp_set_num_threads(NUM_THREADS);
            
    result = new Sudoku();
    result->size = sudoku.size;
    result->grid = new uint8_t*[sudoku.size];
    for (int i = 0; i < sudoku.size; ++i) {
        result->grid[i] = new uint8_t[sudoku.size];
        for (int j = 0; j < sudoku.size; ++j) {
            result->grid[i][j] = sudoku.grid[i][j];
        }
    }

    
    std::vector<Sudoku*> initial_choices = generate_initial_choices(result);
    bool *done = new bool(false);
    for (Sudoku* choice : initial_choices) {
        SerialBacktrackingSolverForParallel solver(*choice);
        std::cout << "Creating solver" << std::endl;
        solver.solved = done;
        solvers.push_back(solver);
    }
}

std::vector<Sudoku*> ParallelBacktrackingSolver::generate_initial_choices(const Sudoku* local_result) {
    std::vector<Sudoku*> initial_choices;
    int row, col;
    if (!find_empty(row, col, local_result)) {
        return initial_choices;
    }

    for (int num = 1; num <= local_result->size; ++num) {
        if (is_valid(row, col, num, local_result)) {
            Sudoku* choice = new Sudoku();
            choice->size = local_result->size;
            choice->grid = new uint8_t*[local_result->size];
            for (int i = 0; i < local_result->size; ++i) {
                choice->grid[i] = new uint8_t[local_result->size];
                for (int j = 0; j < local_result->size; ++j) {
                    choice->grid[i][j] = local_result->grid[i][j];
                }
            }
            choice->grid[row][col] = num;
            initial_choices.push_back(choice);
        }
    }
    return initial_choices;
}

void ParallelBacktrackingSolver::solve() {
    int row, col;
    
    // Find the first empty cell
    // if (!find_empty(row, col)) {
    //     return;  // No empty cells, puzzle is already solved
    // }

    int idx = -1;
    #pragma omp parallel for reduction(+:idx)
    for (int num = 0; num < solvers.size(); ++num) {
        if (idx == -1) {
            bool found = solvers[num].solve2();
            if (found) {
                idx = num;
            }
        }
    }

    if (idx != -1) {
        copy_result(solvers[idx].result);
    }
}

bool ParallelBacktrackingSolver::backtracking(Sudoku* local_result) {
    int row, col;

    // Find the next empty cell
    if (!find_empty(row, col, local_result)) {
        return true;  // No empty cells left, puzzle is solved
    }

    for (int num = 1; num <= 9; ++num) {
        if (is_valid(row, col, num, local_result)) {
            local_result->grid[row][col] = num;

            if (backtracking(local_result)) {
                return true;
            }

            // Backtrack
            local_result->grid[row][col] = 0;
        }
    }
    return false;
}

bool ParallelBacktrackingSolver::is_valid(int row, int col, int num, const Sudoku* local_result) const {
    // Check row and column for conflicts
    for (int i = 0; i < local_result->size; ++i) {
        if (local_result->grid[row][i] == num || local_result->grid[i][col] == num) {
            return false;
        }
    }
    int grid_length = sqrt(local_result->size);
    int startRow = (row / grid_length) * grid_length;
    int startCol = (col / grid_length) * grid_length;
    
    for (int i = 0; i < grid_length; ++i) {
        for (int j = 0; j < grid_length; ++j) {
            if (local_result->grid[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

bool ParallelBacktrackingSolver::find_empty(int &row, int &col, const Sudoku* local_result) const {
    for (row = 0; row < local_result->size; ++row) {
        for (col = 0; col < local_result->size; ++col) {
            if (local_result->grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

void ParallelBacktrackingSolver::copy_result(const Sudoku* local_result) {
    for (int i = 0; i < local_result->size; ++i) {
        for (int j = 0; j < local_result->size; ++j) {
            result->grid[i][j] = local_result->grid[i][j];
        }
    }
}

void ParallelBacktrackingSolver::cleanup(Sudoku* local_result) {
    for (int i = 0; i < local_result->size; ++i) {
        delete[] local_result->grid[i];
    }
    delete[] local_result->grid;
    delete local_result;
}

void ParallelBacktrackingSolver::display() const {

    #pragma omp parallel for
    for (int i = 0; i < result->size; ++i) {
        for (int j = 0; j < result->size; ++j) {
            std::cout << static_cast<int>(result->grid[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}