#include <iostream>
#include <cmath>
#include <stack>
#include <deque>
#include <omp.h>
#include "sudoku_parallel_backtracking_multiblocks.h"

// extern Sudoku* result;

void ParallelBacktrackingMultiBlocksSolver::init(const Sudoku& sudoku) {
    result = new Sudoku();
    result->size = sudoku.size;
    result->grid = new uint8_t*[sudoku.size];
    for (int i = 0; i < sudoku.size; ++i) {
        result->grid[i] = new uint8_t[sudoku.size];
        for (int j = 0; j < sudoku.size; ++j) {
            result->grid[i][j] = sudoku.grid[i][j];
        }
    }

    // Initialize the queue lock
    omp_init_lock(&queue_lock);
    solved = false;
    int row, col;
    if(find_empty(result, row, col)){
        board_queue.push_back({result, row, col});
    } else {
        board_queue.push_back({result, -1, -1});
    }
}

void ParallelBacktrackingMultiBlocksSolver::solve() {
    int iterations = 5;
    bootstrap(iterations); // generate possible boards
    parallel_backtracking();  // Each thread can take all possible sudoku in pool, and execute it 
}
void ParallelBacktrackingMultiBlocksSolver::bootstrap(int iterations){
    // while (!board_queue.empty() && !solved)
    // {
        State current = board_queue.front();
        board_queue.pop_front();
        if (current.row == -1 || current.col == -1) {
            return;
        }
        Sudoku* board = current.board;
        int row = current.row;
        int col = current.col;
        for (int num = 1; num <= board->size; ++num) {
            if (is_valid(board, row, col, num)) {
                Sudoku* new_board = new Sudoku(*board);
                new_board->grid[row][col] = num;

                int next_row, next_col;
                if (find_empty(new_board, next_row, next_col)) {
                    board_queue.push_back({new_board, next_row, next_col});
                } else {
                    if (!solved) {
                        solved = true;
                        copy_result(new_board);
                        cleanup(new_board);
                        break;
                    }
                }
            }
        }
        cleanup(board);
    // }
    
}
void ParallelBacktrackingMultiBlocksSolver::parallel_backtracking() {
    // In openmp, we can not use break, we must traverse all the elements in the queue
    #pragma omp parallel shared(board_queue, solved)
    {
        while(!solved){
            omp_set_lock(&queue_lock);
            if (board_queue.empty()) {  // No work to do
                omp_unset_lock(&queue_lock);
                continue;
            }
            State current = board_queue.front();
            board_queue.pop_front();
            omp_unset_lock(&queue_lock);

            Sudoku* board = current.board;
            int row = current.row; 
            int col = current.col;
            for(int num = 1; num <= board->size; ++num){
                if (is_valid(board, row, col, num)) {
                    board->grid[row][col] = num;

                    int next_row, next_col;
                    if (!find_empty(board, next_row, next_col)){
                        if(!solved){
                            solved = true;
                            copy_result(board);
                        }
                        cleanup(board);
                        break;
                    } 

                    Sudoku* new_board = new Sudoku(*board);
                    new_board->grid[next_row][next_col] = num;
                    omp_set_lock(&queue_lock);
                    board_queue.push_back({new_board, next_row, next_col});
                    omp_unset_lock(&queue_lock);
                }
            }
            cleanup(board);
        }
    }
}

bool ParallelBacktrackingMultiBlocksSolver::is_valid(const Sudoku* local_result, int row, int col, int num) {
    for (int i = 0; i < local_result->size; ++i) {
        if (local_result->grid[row][i] == num || local_result->grid[i][col] == num) {
            return false;
        }
    }

    int grid_length = std::sqrt(local_result->size);
    int start_row = (row / grid_length) * grid_length;
    int start_col = (col / grid_length) * grid_length;

    for (int i = 0; i < grid_length; ++i) {
        for (int j = 0; j < grid_length; ++j) {
            if (local_result->grid[start_row + i][start_col + j] == num) {
                return false;
            }
        }
    }
    return true;
}



bool ParallelBacktrackingMultiBlocksSolver::find_empty(const Sudoku* local_result, int &row, int &col) const {
    for (row = 0; row < local_result->size; ++row) {
        for (col = 0; col < local_result->size; ++col) {
            if (local_result->grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

void ParallelBacktrackingMultiBlocksSolver::copy_result(const Sudoku* local_result) {
    for (int i = 0; i < local_result->size; ++i) {
        for (int j = 0; j < local_result->size; ++j) {
            result->grid[i][j] = local_result->grid[i][j];
        }
    }
}

void ParallelBacktrackingMultiBlocksSolver::cleanup(Sudoku* local_result) {
    for (int i = 0; i < local_result->size; ++i) {
        delete[] local_result->grid[i];
    }
    delete[] local_result->grid;
    delete local_result;
}
void ParallelBacktrackingMultiBlocksSolver::display() const {

    #pragma omp parallel for
    for (int i = 0; i < result->size; ++i) {
        for (int j = 0; j < result->size; ++j) {
            std::cout << static_cast<int>(result->grid[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}