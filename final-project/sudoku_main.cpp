#include "sudoku.h"
#include "sudoku_serial_backtracking.h"
#include "sudoku_serial_bruteforce.h"
#include "sudoku_parallel_backtracking.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_sudoku_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1]; // Get the filename from command-line arguments

    // Dynamically allocate the Sudoku grid
    Sudoku sudoku = loadSudoku(filename);

    // Create a brute force serial solver object
    ParallelBacktrackingSolver* solver = new ParallelBacktrackingSolver(sudoku);
    // SerialBacktrackingSolver* solver = new SerialBacktrackingSolver(sudoku);
    solver->solve();
    
    // Load Sudoku from the file and print it
    std::cout << "Sudoku puzzle loaded from " << filename << ":\n";
    printSudoku(*(solver->result));

    bool valid = isSudokuValid(*(solver->result));
    std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;

    // Clean up memory
    deallocateSudoku(*(solver->result));

    return 0;
}