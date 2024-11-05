#include "sudoku.h"
#include "sudoku_serial_backtracking.h"
#include "sudoku_serial_bruteforce.h"
#include "sudoku_serial_genetic.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_sudoku_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1]; // Get the filename from command-line arguments

    // Dynamically allocate the Sudoku grid
    Sudoku sudoku;
    sudoku.loadSudoku(filename);

    std::cout << "Sudoku puzzle loaded from " << filename << ":\n";
    sudoku.print();

    // Create a brute force serial solver object
    SerialGeneticSolver* solver = new SerialGeneticSolver(sudoku);

    std::cout << "Solving Sudoku using Genetic Algorithm...\n";

    Sudoku result = solver->solve();
    
    // Load Sudoku from the file and print it
    std::cout << "Sudoku puzzle loaded from " << filename << ":\n";
    result.print();

    bool valid = result.isValid();
    std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;

    // Clean up memory
    // deallocateSudoku(*(solver->result));
    delete solver;
    return 0;
}