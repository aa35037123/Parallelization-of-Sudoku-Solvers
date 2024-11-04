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

void Candidate::mutate(double mutation_rate, int mutate_grids, const std::vector<std::vector<uint8_t>>& given) {
    unsigned int seed = 42;
    double r = (double)rand_r(&seed) / RAND_MAX;

    if (r > mutation_rate) {
        return;
    }
    for (int i = 0; i < mutate_grids; ++i) {
        int row = rand_r(&seed) % Nd;
        int column1 = rand_r(&seed) % given[row].size();
        int column2 = rand_r(&seed) % given[row].size();
        std::swap(sudoku.grid[row][column1], sudoku.grid[row][column2]);
    }
}

void Candidate::crossover(const Candidate& parent1, double crossover_rate, double crossover_portion) {
    unsigned int seed = 42;
    double r = (double)rand_r(&seed) / RAND_MAX;

    if (r > crossover_rate) {
        return;
    }

    for (int i = 0; i < Nd; ++i) {
        r = (double)rand_r(&seed) / RAND_MAX;
        if (r < crossover_portion) {
            sudoku.grid[i] = parent1.sudoku.grid[i];
        }
    }
}
