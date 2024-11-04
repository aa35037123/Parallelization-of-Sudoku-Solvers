#ifndef GENETIC_SERIAL_H
#define GENETIC_SERIAL_H

#include <vector>
#include "sudoku.h"

const int Nd = 9;

class Candidate {
public:
    Sudoku sudoku;
    double fitness;
    Candidate(Sudoku sudoku) : sudoku(sudoku), fitness(0.0) {};
    
    void update_fitness();
    void mutate(double mutation_rate, int mutate_grids, const std::vector<std::vector<uint8_t>>& given);
    void crossover(const Candidate& parent1, double crossover_rate, double crossover_portion);

    
};



#endif
