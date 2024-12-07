#ifndef SUDOKU_H
#define SUDOKU_H

#include <cstdint>
#include <string>

// struct Sudoku {
//     int size;
//     uint8_t** grid;
// };

// uint8_t** allocateSudoku(int size);
// void deallocateSudoku(Sudoku sudoku);
// Sudoku loadSudoku(const std::string& filename);
// void printSudoku(const Sudoku& sudoku);
// bool isSudokuValid(const Sudoku& sudoku);


class Sudoku {
public:
    int size;
    uint8_t** grid;

    Sudoku();                            // Default constructor
    Sudoku(int size);                    // Constructor for size initialization and allocation
    Sudoku(int size, uint8_t** grid);    // Constructor for size and grid initialization
    ~Sudoku();                           // Destructor for deallocation

    void loadSudoku(const std::string& filename);  // Static method to load Sudoku from a file

    void print() const;                  // Print Sudoku grid
    bool isValid() const;                // Validate the Sudoku grid
    void copyFrom(const Sudoku& other);  // Copy the grid from another Sudoku object    

    void allocateGrid();                 // Allocate memory for the grid
    // void deallocateGrid();               // Deallocate memory for the grid
    void random_empty_cells(int empty_cells); // Randomly empty cells in the grid
};

#endif