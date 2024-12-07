#include <iostream>
#include <cmath>
#include <queue>
#include <utility>
#include <memory>
#include "sudoku_serial_bruteforce.h"

extern Sudoku* result;

void SerialBruteForceSolver::init(const Sudoku& sudoku) {
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

void SerialBruteForceSolver::solve() {
    int row = 0;
    int col = 0;

    std::cout << "In solving...\n";
    std::queue<State> q; // state: previous find row and column and number
    // q.push(std::make_pair(row, col));
    q.push({result, row, col});
    bool is_solved = false;
    while (!q.empty() && !is_solved) {
        // std::cout << "In while...\n";
        // std::pair<int, int> current = q.front();
        State current = q.front();
        q.pop();
        row = current.row;
        col = current.col;
        result = current.sudoku;
        // std::cout <<"(row, col): " << row << ", " << col << "\n";
        // result->print();
        if (!find_empty(row, col)) {  // auto find the new empty row, col
            std::cout << "Sudoku is solved!\n";
            is_solved = true;
            break;
        } 
        // current.sudoku->print();
        for(int num = 1; num <= result->size; num++){
            if(is_valid(row, col, num)){
                Sudoku* new_sudoku = new Sudoku(result->size, current.sudoku->grid);
                // new_sudoku->copyFrom(*current.sudoku);
                new_sudoku->grid[row][col] = num;
                q.push({new_sudoku, row, col});
            }
        }
    }
    // if sudoku is solved, then free all sudoku elements in queue
    // avoid doesn't free memory because of break
    while (!q.empty()) {
        State current = q.front();
        q.pop();
        delete current.sudoku;
    }
}

bool SerialBruteForceSolver::is_valid(int row, int col, int num) const {
    // Check row and column for conflicts
    for (int i = 0; i < result->size; ++i) {
        if (result->grid[row][i] == num || result->grid[i][col] == num) {
            return false;
        }
    }

    // Check the subgrid for conflicts
    int grid_length = sqrt(result->size);
    int startRow = (row / grid_length) * grid_length;
    int startCol = (col / grid_length) * grid_length;

    for (int i = 0; i < grid_length; ++i) {
        for (int j = 0; j < grid_length; ++j) {
            if (result->grid[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

void SerialBruteForceSolver::display() const {
    for (int i = 0; i < result->size; ++i) {
        for (int j = 0; j < result->size; ++j) {
            std::cout << static_cast<int>(result->grid[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}


bool SerialBruteForceSolver::find_empty(int &row, int &col) const {
    for (row = 0; row < result->size; ++row) {
        for (col = 0; col < result->size; ++col) {
            if (result->grid[row][col] == 0) {\
                return true;  // Found an empty cell
            }
        }
    }
    return false;  // No empty cells found
}