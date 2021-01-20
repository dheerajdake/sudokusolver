#pragma once
#include "common.h"

#define IS_BIT_SET(bit, num)    ((num >> bit) & 1)
#define IS_POWER_OF_TWO(num)    (!(num & (num-1)))

typedef struct _cell
{
    uint32_t value;         // Holds the cell value, empty cell value is 0
                            
                            // For the items below,value [9:1] is represented by bit [9:1]

    uint32_t possibilities; // Holds the possible cell values, bit[9:1]
                            // If bit 0 is set, the cell is not empty

    uint32_t prophecy;      // Holds the values that are guaranteed to appear
                            // only across the cell's row or column. This is useful
                            // in eliminations when solving hard puzzles.
} cell_t;

// Prints all items in grid
void Sudoku_PrintGrid();

// Initialize grid
void Sudoku_InitGrid();

// Updates grid based on the possibility bitMap
void Sudoku_UpdateCellBasedOnPossibilities(uint32_t row, uint32_t col);

// Solver logic
void Sudoku_Solve();
