#include <iostream>
#include <fstream>
#include <cstdint>
#include <unordered_set>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include "sudoku.h"


Sudoku::Sudoku() : size(0) {
    grid = nullptr;
}

Sudoku::Sudoku(int size) : size(size) {
    allocateGrid();
}

Sudoku::Sudoku(int size, uint8_t** grid) : size(size) {

    allocateGrid();
    for (int i = 0; i < size; ++i) {
        memcpy(this->grid[i], grid[i], size * sizeof(uint8_t));
    }

}

void Sudoku::allocateGrid() {
    grid = new uint8_t*[size];
    for (int i = 0; i < size; ++i) {
        grid[i] = new uint8_t[size];
    }
}

Sudoku::~Sudoku() {
    for (int i = 0; i < size; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Sudoku::loadSudoku(const std::string& filename) {
    std::ifstream file(filename);

    int gridSize;

    if (!file || !(file >> gridSize) || gridSize <= 0) {
        std::cerr << "Invalid file or grid size." << std::endl;
        return;
    }

    if (size != gridSize){
        size = gridSize;
        allocateGrid();
    }

    // Read each cell value into the 2D array
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int value;
            file >> value;
            grid[row][col] = static_cast<uint8_t>(value);
        }
    }
    file.close();
}

void Sudoku::print() const {
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            printf("%3d", static_cast<int>(grid[row][col]));
        }
        printf("\n");
    }
}


bool Sudoku::isValid() const {
    // Check each row
    for (int row = 0; row < size; ++row) {
        std::unordered_set<int> rowSet;
        for (int col = 0; col < size; ++col) {
            int value = grid[row][col];
            if (value < 1 || value > size || !rowSet.insert(value).second) {
                return false;
            }
        }
    }

    // Check each column
    for (int col = 0; col < size; ++col) {
        std::unordered_set<int> colSet;
        for (int row = 0; row < size; ++row) {
            int value = grid[row][col];
            if (value < 1 || value > size || !colSet.insert(value).second) {
                return false;
            }
        }
    }

    // Check each sub-grid
    int subGridSize = static_cast<int>(std::sqrt(size));
    for (int boxRow = 0; boxRow < size; boxRow += subGridSize) {
        for (int boxCol = 0; boxCol < size; boxCol += subGridSize) {
            std::unordered_set<int> boxSet;
            for (int row = 0; row < subGridSize; row++) {
                for (int col = 0; col < subGridSize; col++) {
                    int value = grid[boxRow + row][boxCol + col];
                    if (value < 1 || value > size || !boxSet.insert(value).second) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Sudoku::copyFrom(const Sudoku& other) {
    if (grid == nullptr) {
        size = other.size;
        allocateGrid();
    } 
    else if (size != other.size){
        std::cerr << "Grid sizes do not match when copying.\n";
        return;
    }
    for (int i = 0; i < size; ++i) {
        memcpy(grid[i], other.grid[i], size * sizeof(uint8_t));
    }
}

void Sudoku::random_empty_cells(int empty_cells) {
    std::vector<int> empty_indices;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            empty_indices.push_back(i * size + j);
        }
    }
    std::srand(std::time(0));
    std::random_shuffle(empty_indices.begin(), empty_indices.end());
    for (int i = 0; i < empty_cells; ++i) {
        int row = empty_indices[i] / size;
        int col = empty_indices[i] % size;
        grid[row][col] = 0;
    }
}
