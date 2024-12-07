#include <iostream>
#include <cmath>
#include <omp.h>
#include "sudoku_parallel_backtracking.h"

extern Sudoku* result;

void ParallelBacktrackingSolver::init(const Sudoku& sudoku) {
    result = new Sudoku();
    result->size = sudoku.size;
    result->grid = new uint8_t*[sudoku.size];
    for (int i = 0; i < sudoku.size; ++i) {
        result->grid[i] = new uint8_t[sudoku.size];
        for (int j = 0; j < sudoku.size; ++j) {
            result->grid[i][j] = sudoku.grid[i][j];
        }
    }
}

void ParallelBacktrackingSolver::solve() {
    int row, col;
    
    // Find the first empty cell
    // if (!find_empty(row, col)) {
    //     return;  // No empty cells, puzzle is already solved
    // }

    bool solution_found = false;

    // Parallelize the initial choices for the first empty cell
    #pragma omp parallel for shared(solution_found)
    for (int num = 1; num <= 9; ++num) {
        if (solution_found) continue;  // Stop if another thread has found a solution
        
        Sudoku* local_result = new Sudoku();
        local_result->size = result->size;
        local_result->grid = new uint8_t*[result->size];
        for (int i = 0; i < result->size; ++i) {
            local_result->grid[i] = new uint8_t[result->size];
            for (int j = 0; j < result->size; ++j) {
                local_result->grid[i][j] = result->grid[i][j];
            }
        }

        if (is_valid(row, col, num, local_result)) {
            local_result->grid[row][col] = num;

            // Recursively solve using this initial choice
            if (backtracking(local_result)) {
                #pragma omp critical
                {
                    if (!solution_found) {
                        solution_found = true;
                        copy_result(local_result);
                    }
                }
            }
        }
        cleanup(local_result);  // Free memory allocated for local_result
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