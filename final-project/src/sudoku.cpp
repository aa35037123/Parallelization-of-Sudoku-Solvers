#include <iostream>
#include <fstream>
#include <cstdint>
#include <unordered_set>
#include <cmath>
#include "sudoku.h"


uint8_t** allocateSudoku(int size) {
    // Dynamically allocate a 2D array
    uint8_t** sudoku = new uint8_t*[size];
    for (int i = 0; i < size; ++i) {
        sudoku[i] = new uint8_t[size];
    }
    return sudoku;
}

void deallocateSudoku(Sudoku sudoku) {
    // Free the dynamically allocated 2D array
    for (int i = 0; i < sudoku.size; ++i) {
        delete[] sudoku.grid[i];
    }
    //delete[] sudoku.grid;
}

Sudoku loadSudoku(const std::string& filename) {
    std::ifstream file(filename);

    int gridSize;

    if (!file || !(file >> gridSize) || gridSize <= 0) {
        std::cerr << "Invalid file or grid size." << std::endl;
        return Sudoku{0, nullptr};
    }

    uint8_t** sudoku = allocateSudoku(gridSize);

    // Read each cell value into the 2D array
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int value;
            file >> value;
            sudoku[row][col] = static_cast<uint8_t>(value);
        }
    }
    file.close();
    return Sudoku{gridSize, sudoku};
}

void printSudoku(const Sudoku& sudoku) {
    for (int row = 0; row < sudoku.size; ++row) {
        for (int col = 0; col < sudoku.size; ++col) {
            printf("%3d", static_cast<int>(sudoku.grid[row][col]));
        }
        printf("\n");
    }
}


bool isSudokuValid(const Sudoku& sudoku) {
    int n = sudoku.size;

    // Check each row
    for (int row = 0; row < n; ++row) {
        std::unordered_set<int> rowSet;
        for (int col = 0; col < n; ++col) {
            int value = sudoku.grid[row][col];
            if (value < 1 || value > n || !rowSet.insert(value).second) {
                return false;
            }
        }
    }

    // Check each column
    for (int col = 0; col < n; ++col) {
        std::unordered_set<int> colSet;
        for (int row = 0; row < n; ++row) {
            int value = sudoku.grid[row][col];
            if (value < 1 || value > n || !colSet.insert(value).second) {
                return false;
            }
        }
    }

    // Check each sub-grid
    int subGridSize = static_cast<int>(std::sqrt(n));
    for (int boxRow = 0; boxRow < n; boxRow += subGridSize) {
        for (int boxCol = 0; boxCol < n; boxCol += subGridSize) {
            std::unordered_set<int> boxSet;
            for (int row = 0; row < subGridSize; row++) {
                for (int col = 0; col < subGridSize; col++) {
                    int value = sudoku.grid[boxRow + row][boxCol + col];
                    if (value < 1 || value > n || !boxSet.insert(value).second) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}
