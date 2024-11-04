#include "sudoku.h"
#include "sudoku_backtracking.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_sudoku_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1]; // Get the filename from command-line arguments


    // Dynamically allocate the Sudoku grid
    Sudoku sudoku = loadSudoku(filename);

    // Load Sudoku from the file and print it
    std::cout << "Sudoku puzzle loaded from " << filename << ":\n";
    printSudoku(sudoku);

    bool valid = isSudokuValid(sudoku);
    std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;

    // Clean up memory
    deallocateSudoku(sudoku);

    return 0;
}