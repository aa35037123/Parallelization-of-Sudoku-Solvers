#include "sudoku.h"
#include "sudoku_serial_backtracking.h"
#include "sudoku_serial_bruteforce.h"
#include "sudoku_serial_genetic.h"
#include "sudoku_parallel_bruteforce.h"
#include "sudoku_parallel_bruteforce_mpi.h"
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <mpi.h>
#include "CycleTimer.h"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <path_to_sudoku_file> [--algorithm <number>]\n"
              << "Available algorithms:\n"
              << "  1  - Serial implementation using backtracking\n"
              << "  2  - Serial implementation using brute force\n"
              << "  3  - Parallel MPI implementation using brute force \n";
}

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Only rank 0 processes command line arguments
    std::string filename;
    int algorithmChoice = 8; // default to MPI implementation
    
    if (rank == 0) {
        if (argc < 2) {
            printUsage(argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        // Parse command line arguments
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--algorithm") == 0) {
                if (i + 1 < argc) {
                    algorithmChoice = std::atoi(argv[++i]);
                } else {
                    std::cerr << "Error: --algorithm requires a value\n";
                    printUsage(argv[0]);
                    MPI_Abort(MPI_COMM_WORLD, 1);
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
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
    }

    // Broadcast algorithm choice to all processes
    MPI_Bcast(&algorithmChoice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Load the Sudoku puzzle on rank 0 and broadcast it
    Sudoku sudoku;
    if (rank == 0) {
        sudoku.loadSudoku(filename);
    }
    // You'll need to implement MPI broadcasting of the sudoku grid
    MPI_Bcast(&sudoku.size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        sudoku.grid = new uint8_t*[sudoku.size];
        for (int i = 0; i < sudoku.size; i++) {
            sudoku.grid[i] = new uint8_t[sudoku.size];
        }
    }
    for (int i = 0; i < sudoku.size; i++) {
        MPI_Bcast(sudoku.grid[i], sudoku.size, MPI_UINT8_T, 0, MPI_COMM_WORLD);
    }

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
        case 3:
            solver = std::make_unique<MPIBruteForceSolver>(sudoku);
            algorithmName = "Parallel MPI bruteforce (BFS)";
            break;
        default:
            if (rank == 0) {
                std::cerr << "Error: Unknown algorithm number '" << algorithmChoice << "'\n";
                printUsage(argv[0]);
            }
            MPI_Finalize();
            return 1;
    }

    solver->solve();

    double endTime = CycleTimer::currentSeconds();

    // Only rank 0 prints results
    if (rank == 0) {
        std::cout << "Time: " << (endTime - startTime) * 1000 << " ms\n";
        std::cout << "Using algorithm: " << algorithmName << "\n";
        std::cout << "Sudoku puzzle loaded from: " << filename << "\n";
        solver->result->print();

        bool valid = solver->result->isValid();
        std::cout << "Sudoku is " << (valid ? "valid." : "invalid.") << std::endl;
    }

    // Cleanup and finalize MPI
    MPI_Finalize();
    return 0;
}