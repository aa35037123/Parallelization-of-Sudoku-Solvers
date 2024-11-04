#include <iostream>
#include <fstream>
#include <cmath>
#include "sudoku.h"
#include "SudokuSolver_SerialBruteForce.h"

class SerialBruteForceSolver: public SudokuSolver{
public:
    Sudoku* result;
    SerialBruteForceSolver() : result(nullptr) {}
    SerialBruteForceSolver(const Sudoku& sudoku) : result(nullptr) {
        load(sudoku);
    }

    void load(const Sudoku& sudoku);

    bool is_valid(int row, int col, int num);

    bool solve();

    void display();

private:
    bool find_empty(int &row, int &col);
};