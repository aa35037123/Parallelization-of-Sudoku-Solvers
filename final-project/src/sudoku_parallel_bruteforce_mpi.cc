// #include "sudoku_parallel_bruteforce.h"
// #include <cmath>
// #include <algorithm>

// MPIBruteForceSolver::MPIBruteForceSolver() : solution_found(false) {
//     result = nullptr;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &world_size);
// }

// MPIBruteForceSolver::MPIBruteForceSolver(const Sudoku& sudoku) : MPIBruteForceSolver() {
//     init(sudoku);
// }

// MPIBruteForceSolver::~MPIBruteForceSolver() {
//     for (auto board : local_boards) {
//         delete board;
//     }
// }

// void MPIBruteForceSolver::init(const Sudoku& sudoku) {
//     if (result != nullptr) {
//         delete result;
//     }
//     result = new Sudoku(sudoku);
//     solution_found = false;

//     if (rank == 0) {
//         // Root process generates initial boards
//         std::vector<Sudoku*> initial_boards = init_unsolved_boards(0, result);
//         distribute_work(initial_boards);
        
//         // Cleanup initial boards after distribution
//         for (auto board : initial_boards) {
//             delete board;
//         }
//     } else {
//         // Other processes wait to receive their work
//         distribute_work(std::vector<Sudoku*>());
//     }
// }

// void MPIBruteForceSolver::solve() {
//     // Process local boards
//     process_local_boards(local_boards);
    
//     // Gather results from all processes
//     gather_results();
// }

// void MPIBruteForceSolver::process_local_boards(const std::vector<Sudoku*>& boards) {
//     bool local_solution = false;
//     Sudoku* solution_board = nullptr;

//     for (auto board : boards) {
//         SerialBruteForceSolver solver(*board);
//         if (solver.solve()) {
//             local_solution = true;
//             solution_board = new Sudoku(*solver.get_result());
//             break;
//         }
//     }

//     // Pack solution status and board
//     std::vector<uint8_t> buffer;
//     if (local_solution) {
//         serialize_sudoku(solution_board, buffer);
//     }
    
//     int buffer_size = buffer.size();
//     MPI_Bcast(&buffer_size, 1, MPI_INT, rank, MPI_COMM_WORLD);
    
//     if (buffer_size > 0) {
//         buffer.resize(buffer_size);
//         MPI_Bcast(buffer.data(), buffer_size, MPI_BYTE, rank, MPI_COMM_WORLD);
//     }

//     delete solution_board;
// }

// void MPIBruteForceSolver::distribute_work(const std::vector<Sudoku*>& initial_boards) {
//     if (rank == 0) {
//         // Calculate work distribution
//         int boards_per_process = initial_boards.size() / world_size;
//         int remaining_boards = initial_boards.size() % world_size;
        
//         // Distribute boards to other processes
//         int current_pos = 0;
//         for (int i = 0; i < world_size; i++) {
//             int boards_to_send = boards_per_process + (i < remaining_boards ? 1 : 0);
            
//             if (i == rank) {
//                 // Keep local boards for root process
//                 for (int j = 0; j < boards_to_send; j++) {
//                     local_boards.push_back(new Sudoku(*initial_boards[current_pos + j]));
//                 }
//             } else {
//                 // Send boards to other processes
//                 for (int j = 0; j < boards_to_send; j++) {
//                     std::vector<uint8_t> buffer;
//                     serialize_sudoku(initial_boards[current_pos + j], buffer);
//                     int buffer_size = buffer.size();
//                     MPI_Send(&buffer_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//                     MPI_Send(buffer.data(), buffer_size, MPI_BYTE, i, 0, MPI_COMM_WORLD);
//                 }
//             }
//             current_pos += boards_to_send;
//         }
//     } else {
//         // Receive boards from root process
//         while (true) {
//             int buffer_size;
//             MPI_Status status;
//             MPI_Recv(&buffer_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            
//             if (buffer_size == 0) break;
            
//             std::vector<uint8_t> buffer(buffer_size);
//             MPI_Recv(buffer.data(), buffer_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
            
//             Sudoku* board = deserialize_sudoku(buffer);
//             local_boards.push_back(board);
//         }
//     }
// }

// void MPIBruteForceSolver::gather_results() {
//     if (rank == 0) {
//         // Root process receives and processes solutions
//         std::vector<uint8_t> solution_buffer;
//         MPI_Status status;
        
//         for (int i = 1; i < world_size; i++) {
//             int buffer_size;
//             MPI_Recv(&buffer_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            
//             if (buffer_size > 0) {
//                 solution_buffer.resize(buffer_size);
//                 MPI_Recv(solution_buffer.data(), buffer_size, MPI_BYTE, i, 0, MPI_COMM_WORLD, &status);
                
//                 Sudoku* solution = deserialize_sudoku(solution_buffer);
//                 copy_result(*solution);
//                 delete solution;
//                 solution_found = true;
//                 break;
//             }
//         }
//     } else {
//         // Worker processes send their solutions to root
//         int buffer_size = 0;
//         std::vector<uint8_t> buffer;
        
//         if (solution_found) {
//             serialize_sudoku(result, buffer);
//             buffer_size = buffer.size();
//         }
        
//         MPI_Send(&buffer_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//         if (buffer_size > 0) {
//             MPI_Send(buffer.data(), buffer_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
//         }
//     }
// }

// void MPIBruteForceSolver::serialize_sudoku(const Sudoku* sudoku, std::vector<uint8_t>& buffer) {
//     buffer.clear();
//     buffer.push_back(sudoku->size);
    
//     for (int i = 0; i < sudoku->size; i++) {
//         for (int j = 0; j < sudoku->size; j++) {
//             buffer.push_back(sudoku->grid[i][j]);
//         }
//     }
// }

// Sudoku* MPIBruteForceSolver::deserialize_sudoku(const std::vector<uint8_t>& buffer) {
//     Sudoku* sudoku = new Sudoku();
//     sudoku->size = buffer[0];
    
//     sudoku->grid = new uint8_t*[sudoku->size];
//     for (int i = 0; i < sudoku->size; i++) {
//         sudoku->grid[i] = new uint8_t[sudoku->size];
//         for (int j = 0; j < sudoku->size; j++) {
//             sudoku->grid[i][j] = buffer[1 + i * sudoku->size + j];
//         }
//     }
//     return sudoku;
// }

// // The rest of the helper functions (is_valid, find_empty, etc.) remain the same as in the pthread version