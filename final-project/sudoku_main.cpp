#include "sudoku.h"
#include "sudoku_serial_backtracking.h"
#include "sudoku_serial_bruteforce.h"
#include "sudoku_serial_genetic.h"
#include "sudoku_parallel_backtracking.h"
#include "sudoku_parallel_backtracking_multiblocks.h"
#include "sudoku_parallel_bruteforce_omp.h"
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include "CycleTimer.h"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <path_to_sudoku_file> [--algorithm <number>]\n"
              << "Available algorithms:\n"
              << "  1  - Serial implementation using backtracking\n"
              << "  2  - Serial implementation using brute force\n"
              << "  3  - Serial implementation using genetic algorithm\n"
              << "  4  - Parallel implementation using backtracking algorithm\n"
              << "  5  - Parallel implementation using backtracking - multiblocks algorithm\n"
              << "  6  - Parallel OMP implementation using brute force algorithm\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    // Default values
    std::string filename;
    int algorithmChoice = 1; // default serial_backtracking

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--algorithm") == 0) {
            if (i + 1 < argc) {
                algorithmChoice = std::atoi(argv[++i]);
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
    std::unique_ptr<SudokuSolver> solver;
    std::string algorithmName;

    double startTime = CycleTimer::currentSeconds();

    // Create the appropriate solver based on the algorithm choice
    switch (algorithmChoice) {
        case 1: // Serial backtracking
            solver = std::make_unique<SerialBacktrackingSolver>(sudoku);
            algorithmName = "Serial backtracking (DFS)";
            break;
        case 2: // Serial brute force
            solver = std::make_unique<SerialBruteForceSolver>(sudoku);
            algorithmName = "Serial bruteforce (BFS)";
            break;
        case 3: // Serial genetic algorithm
            // solver = std::make_unique<SerialGeneticSolver>(sudoku);
            break;
        case 4:
            solver = std::make_unique<ParallelBacktrackingSolver>(sudoku);
            algorithmName = "Parallel backtracking (DFS)";
            break;
        case 5:
            solver = std::make_unique<ParallelBacktrackingMultiBlocksSolver>(sudoku);
            break;
        case 6:
            solver = std::make_unique<OMPParallelBruteForceSolver>(sudoku);
            algorithmName = "Parallel OMP bruteforce (BFS)";
            break;
        default:
            std::cerr << "Error: Unknown algorithm number '" << algorithmChoice << "'\n";
            algorithmName = "Unknown";
            printUsage(argv[0]);
            return 1;
    }

    solver->solve();

    double endTime = CycleTimer::currentSeconds();
    std::cout << "Time: " << (endTime - startTime) * 1000 << " ms\n";
    
    // Print results
    std::cout << "Using algorithm: " << algorithmName << "\n";
    std::cout << "Sudoku puzzle loaded from: " << filename << "\n";
    solver->result->print();

    bool valid = solver->result->isValid();
    std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;

    return 0;
}
