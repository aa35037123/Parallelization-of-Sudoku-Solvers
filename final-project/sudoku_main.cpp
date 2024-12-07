#include "sudoku.h"
#include "sudoku_serial_backtracking.h"
#include "sudoku_serial_bruteforce.h"
#include "sudoku_serial_genetic.h"
//#include "sudoku_parallel_backtracking.h"
#include <iostream>
#include <memory>
#include <string>
#include <cstring>

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <path_to_sudoku_file> [--algorithm <algorithm_name>]\n"
              << "Available algorithms:\n"
              << "  parallel_backtracking  - Parallel implementation using backtracking (default)\n"
              << "  serial_backtracking    - Serial implementation using backtracking\n"
              << "  serial_bruteforce      - Serial implementation using brute force\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    // Default values
    std::string filename;
    std::string algorithmChoice = "parallel_backtracking";  // default algorithm

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--algorithm") == 0) {
            if (i + 1 < argc) {
                algorithmChoice = argv[++i];
            } else {
                std::cerr << "Error: --algorithm requires a value\n";
                printUsage(argv[0]);
                return 1;
            }
        } else {
            // Assume this is the filename
            filename = argv[i];
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: No input file specified\n";
        printUsage(argv[0]);
        return 1;
    }

    // Load the Sudoku puzzle
    Sudoku sudoku;
    sudoku.loadSudoku(filename);
<<<<<<< HEAD
    
    // Create a brute force serial solver object
    // SerialBruteForceSolver* solver = new SerialBruteForceSolver(sudoku);
    SerialBacktrackingSolver* solver = new SerialBacktrackingSolver(sudoku);
=======
    std::unique_ptr<SudokuSolver> solver;

    // Create the appropriate solver based on the algorithm choice
    // if (algorithmChoice == "parallel_backtracking") {
    //     solver = std::make_unique<ParallelBacktrackingSolver>(sudoku);
    // } else 
    if (algorithmChoice == "serial_backtracking") {
        solver = std::make_unique<SerialBacktrackingSolver>(sudoku);
    } else if (algorithmChoice == "serial_bruteforce") {
        solver = std::make_unique<SerialBruteForceSolver>(sudoku);
    }
    else {
        std::cerr << "Error: Unknown algorithm '" << algorithmChoice << "'\n";
        printUsage(argv[0]);
        return 1;
    }

>>>>>>> 80982b4523b15b352f818654b9d61ae550bb7e80
    solver->solve();
    
    // Print results
    std::cout << "Using algorithm: " << algorithmChoice << "\n";
    std::cout << "Sudoku puzzle loaded from " << filename << ":\n";
    solver->result->print();

    bool valid = solver->result->isValid();
    std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;

    // clear solver
    free((solver->result));
    //deallocateSudoku(*(solver->result));
    return 0;
}