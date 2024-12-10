#include <iostream>
#include <cmath>
#include <queue>
#include <omp.h>
#include "sudoku_parallel_bruteforce_omp.h"
using namespace std;
#define NUM_THREADS 4

bool SerialBruteforceSolverForParallel::solve2() {
    int row = 0;
    int col = 0;

    std::queue<vector<State>> q; // state: previous find row and column and number
    Sudoku blank_sudoku;
    blank_sudoku.copyFrom(*result);

    vector<pair<int, int>> empty_cells;
    for(int i = 0; i < result->size; i++){
        for(int j = 0; j < result->size; j++){
            if(result->grid[i][j] == 0){
                empty_cells.push_back(make_pair(i, j));
            }
        }
    }
    //= new Sudoku(result->size, result->grid);
    // q.push(std::make_pair(row, col));
    // push first cell state
    bool is_solved = !find_empty(row, col, blank_sudoku);
    //std::cout << row << " " << col << "\n";
    if (!is_solved) {
        for (uint8_t num = 1; num <= result->size; num++) {
            if (is_valid(row, col, num, blank_sudoku))
                q.push({{num}});
        }
    }
    while (!q.empty() && !is_solved) {
        //std::cout << "In while...\n";
        // std::pair<int, int> current = q.front();
        vector<State> current = q.front();
        q.pop();
        for (auto i = 0; i < current.size(); i++)
            blank_sudoku.grid[empty_cells[i].first][empty_cells[i].second] = current[i].num;
        // std::cout <<"(row, col): " << row << ", " << col << "\n";
        // result->print();
        row = empty_cells[current.size() - 1].first;
        col = empty_cells[current.size() - 1].second;
        if (!find_empty(row, col, blank_sudoku)) {  // auto find the new empty row, col
            is_solved = true;
            break;
        } 
        // current.sudoku->print();
        for(uint8_t num = 1; num <= result->size; num++){

            if(is_valid(row, col, num, blank_sudoku)){
                current.push_back({num});
                q.push(current);
                current.pop_back();
            }
        }
        //blank_sudoku.print();
        for (auto i = 0; i < current.size(); i++)
            blank_sudoku.grid[empty_cells[i].first][empty_cells[i].second] = 0;
    }
    // if sudoku is solved, then free all sudoku elements in queue
    // avoid doesn't free memory because of break
    while (!q.empty())
        q.pop();
    
    if (is_solved) {
        for (int i = 0;i<result->size;i++){
            for(int j = 0;j<result->size;j++){
                result->grid[i][j] = blank_sudoku.grid[i][j];
            }
        }
    }
    return is_solved;
}

void OMPParallelBruteForceSolver::init(const Sudoku& sudoku) {
    if (result != nullptr) {
        delete result;
        result = nullptr;
    }
    unsolvedBoards.clear();

    omp_set_dynamic(0);
    omp_set_num_threads(NUM_THREADS);
    
    result = new Sudoku(sudoku);

    std::vector<Sudoku*> initial_choices = init_unsolved_boards(0, result);
    //unsolvedBoards.reserve(initial_choices.size());
    for (Sudoku* choice : initial_choices) {
        SerialBruteforceSolverForParallel* sbf = new SerialBruteforceSolverForParallel(*choice);
        unsolvedBoards.push_back(sbf);
        delete choice;
    }    
}

std::vector<Sudoku*> OMPParallelBruteForceSolver::init_unsolved_boards(int current_strap, const Sudoku* local_result) {
    std::vector<Sudoku*> initial_choices;
    if (current_strap == bootstrap) {
        initial_choices.push_back(new Sudoku(*local_result));
        return initial_choices;
    }

    int row = 0;
    int col = 0;
    if (!find_empty(row, col, local_result)) {
        return initial_choices;
    }
    for (int num = 1; num <= local_result->size; ++num) {
        if (is_valid(row, col, num, local_result)) {
            Sudoku* choice = new Sudoku();
            choice->size = local_result->size;
            choice->grid = new uint8_t*[local_result->size];
            for (int i = 0; i < local_result->size; ++i) {
                choice->grid[i] = new uint8_t[local_result->size];
                std::copy(local_result->grid[i], 
                         local_result->grid[i] + local_result->size,
                         choice->grid[i]);
            }
            choice->grid[row][col] = num;
            
            std::vector<Sudoku*> next_choices = init_unsolved_boards(current_strap + 1, choice);
            if (next_choices.empty())
                continue;
            for (Sudoku* next_choice : next_choices) {
                initial_choices.push_back(next_choice);
            }
        }
    }
    return initial_choices;
}

void OMPParallelBruteForceSolver::solve() {
    int idx = -1;
    //std::cout << unsolvedBoards.size() << std::endl;

    #pragma omp parallel for shared(idx)
    for (int num = 0; num < unsolvedBoards.size(); ++num) {
        bool found = unsolvedBoards[num]->solve2();
        if (found) {
            #pragma omp critical
            {
                idx = num;
            }
        }
    }

    if (idx != -1) {
        copy_result(*(unsolvedBoards[idx]->result));
    }
    else {
        cout << "No solution found" << endl;
    }
}

bool OMPParallelBruteForceSolver::is_valid(int row, int col, int num, const Sudoku* sudoku) const {
    // Check row and column for conflicts
    for (int i = 0; i < sudoku->size; ++i) {
        if (sudoku->grid[row][i] == num || sudoku->grid[i][col] == num) {
            return false;
        }
    }

    // Check the subgrid for conflicts
    int grid_length = sqrt(sudoku->size);
    int startRow = (row / grid_length) * grid_length;
    int startCol = (col / grid_length) * grid_length;

    for (int i = 0; i < grid_length; ++i) {
        for (int j = 0; j < grid_length; ++j) {
            if (sudoku->grid[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

void OMPParallelBruteForceSolver::copy_result(const Sudoku& sudoku) {
    for (int i = 0; i < sudoku.size; ++i) {
        for (int j = 0; j < sudoku.size; ++j) {
            result->grid[i][j] = sudoku.grid[i][j];
        }
    }
}

bool OMPParallelBruteForceSolver::find_empty(int &row, int &col, const Sudoku* local_result) const {
     // First check the remaining cells in the current row
    for (int c = col; c < local_result->size; ++c) {
        if (local_result->grid[row][c] == 0) {
            col = c;
            return true;
        }
    }
    
    // Then check subsequent rows
    for (int r = row + 1; r < local_result->size; ++r) {
        for (int c = 0; c < local_result->size; ++c) {
            if (local_result->grid[r][c] == 0) {
                row = r;
                col = c;
                return true;
            }
        }
    }
    return false;
}