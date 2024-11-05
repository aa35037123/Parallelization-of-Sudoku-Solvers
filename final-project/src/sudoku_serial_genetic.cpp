#include "sudoku_serial_genetic.h"
#include <set>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdlib.h>


void Candidate::update_fitness() {
    std::vector<int> column_count(sudoku.size, 0);
    std::vector<int> block_count(sudoku.size, 0);
    double column_sum = 0.0;
    double block_sum = 0.0;

    // Calculate column fitness
    for (int i = 0; i < sudoku.size; ++i) {
        for (int j = 0; j < sudoku.size; ++j) {
            column_count[sudoku.grid[j][i] - 1]++;
        }
        for (int j = 0; j < sudoku.size; ++j) {
            if (column_count[j] > 0) {
                column_sum += 1.0 / (column_count[j]);
            }
        }
        std::fill(column_count.begin(), column_count.end(), 0);
    }

    // Calculate block fitness
    for (int i = 0; i < sudoku.size; i += 3) {
        for (int j = 0; j < sudoku.size; j += 3) {
            for (int x = 0; x < 3; ++x) {
                for (int y = 0; y < 3; ++y) {
                    block_count[sudoku.grid[i + x][j + y] - 1]++;
                }
            }
            for (int x = 0; x < sudoku.size; ++x) {
                if (block_count[x] > 0) {
                    block_sum += 1.0 / (block_count[x]);
                }
            }
            std::fill(block_count.begin(), block_count.end(), 0);
        }
    }

    fitness = column_sum * block_sum;
}

void Candidate::mutate(int mutate_grids, const std::vector<std::vector<uint8_t>>& given) {
    unsigned int seed = 42;
    double r;
    for (int i = 0; i < mutate_grids; ++i) {
        int row = rand_r(&seed) % sudoku.size;
        int column1 = rand_r(&seed) % given[row].size();
        int column2 = rand_r(&seed) % given[row].size();
        std::swap(sudoku.grid[row][column1], sudoku.grid[row][column2]);
    }
}

void Candidate::crossover(const Candidate& parent1, double crossover_portion) {
    unsigned int seed = 42;
    double r;

    for (int i = 0; i < sudoku.size; ++i) {
        r = (double)rand_r(&seed) / RAND_MAX;
        if (r < crossover_portion) {
            sudoku.grid[i] = parent1.sudoku.grid[i];
        }
    }
}

void Candidate::initialize() {
    std::vector<bool> row(sudoku.size);
    std::vector<uint8_t> row_miss;
    for (int i = 0; i < sudoku.size; ++i) {
        std::fill(row.begin(), row.end(), true);
        for (int j = 0; j < sudoku.size; ++j) {
            if (sudoku.grid[i][j] != 0)
                row[sudoku.grid[i][j] - 1] = false;
        }
        for (int j = 0; j < sudoku.size; ++j) {
            if (row[j]) {
                row_miss.push_back(j+1);
            }
        }
        std::random_shuffle(row_miss.begin(), row_miss.end());
        for (int j = 0; j < sudoku.size; ++j) {
            if (sudoku.grid[i][j] == 0) {
                sudoku.grid[i][j] = row_miss.back();
                row_miss.pop_back();
            }
        }
    }
}

void Population::selection(int selection_size) {
    population.sort([](Candidate* a, Candidate* b) {
        return a->fitness > b->fitness;
    });
    while (population.size() > population_size) {
        delete population.back();
        population.pop_back();
    }
}

void Population::crossover(int crossover_amount, double crossover_portion){
    for (int i = 0; i < crossover_amount; i++){
        std::list<Candidate*>::iterator it1 = population.begin();
        std::list<Candidate*>::iterator it2 = population.begin();
        std::advance(it1, rand() % population.size());
        std::advance(it2, rand() % population.size());
        Candidate* parent1 = new Candidate((*it1)->sudoku);
        Candidate* parent2 = *it2;
        parent1->crossover(*parent2, crossover_portion);
        population.push_back(parent1);
    }
}

void Population::mutate(int mutate_amount, int mutate_grids, const std::vector<std::vector<uint8_t>>& given) {
    for (int i = 0; i < mutate_amount; i++) {
        std::list<Candidate*>::iterator it = population.begin();
        std::advance(it, rand() % population.size());
        Candidate* new_candidate = new Candidate((*it)->sudoku);
        new_candidate->mutate(mutate_grids, given);
        population.push_back(new_candidate);
    }
}

void Population::evolve(int selection_size, int crossover_amount, double crossover_portion, int mutate_amount, int mutate_grids, const std::vector<std::vector<uint8_t>>& given) {
    for (Candidate* candidate : population) {
        candidate->update_fitness();
    }
    selection(selection_size);
    crossover(crossover_amount, crossover_portion);
    mutate(mutate_amount, mutate_grids, given);
    generation++;
}
