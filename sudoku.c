#include "sudoku.h"
#include "testCases.h"

// Nine 3x3 Mini Grids start coordinates
uint32_t miniGridIndices[9][2] =
{
    {0, 0}, {0, 3}, {0, 6},
    {3, 0}, {3, 3}, {3, 6},
    {6, 0}, {6, 3}, {6, 6}
};

// Holds the unsolved puzzle values
cell_t grid[9][9];

// Tracks the unsolved cells in puzzle
uint32_t cellsToFill = 0;

// Prints the grid values
void Sudoku_PrintGrid()
{
    printf("\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("{%d, %03x, %03x}    ", grid[i][j].value, grid[i][j].possibilities, grid[i][j].prophecy);
        }
        printf("\n");
    }
}

void Sudoku_InitGrid()
{
    // Read suduko
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            grid[i][j].value = suduko[i][j];

            if (grid[i][j].value != 0)
            {
                grid[i][j].possibilities = 1;
                grid[i][j].prophecy = 1;
            }
            else
            {
                cellsToFill++;
                grid[i][j].possibilities = 0;
                grid[i][j].prophecy = 0;
            }
        }
    }
}

void Sudoku_UpdateCellBasedOnPossibilities(uint32_t row, uint32_t col)
{
    if (IS_POWER_OF_TWO(grid[row][col].possibilities))
    {
        // Find the bit which is set
        for (uint32_t bit = 1; bit < 10; bit++)
        {
            if (IS_BIT_SET(bit, grid[row][col].possibilities))
            {
                grid[row][col].value = bit;
                grid[row][col].possibilities = 1;
                grid[row][col].prophecy = 1;
                cellsToFill--;
                DEBUG_PRINT("Setting value %d based on power of two at [%d, %d] = %d\n", bit, row, col);

                if (grid[row][col].value != answer[row][col])
                {
                    ASSERT(false, "Wrong value predicted\n");
                }

                break;
            }
        }
    }
}

void Sudoku_Solve()
{
    // Guess grid
    int iterCount = 0;
    DEBUG_PRINT("Empty cells %d\n", cellsToFill);

    uint32_t prevCellsToFill;

    // Don't exit until the puzzle is solved
    while (cellsToFill != 0)
    {
        // Keep track of unsolved cells
        prevCellsToFill = cellsToFill;

        // Scan through the grid
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // Skip solved cells
                if (IS_BIT_SET(0, grid[i][j].possibilities))
                {
                    continue;
                }

                // Get cell's miniGrid start coordinates
                uint32_t idx = (j / 3) + (3 * (i / 3));
                int rS = miniGridIndices[idx][0];
                int cS = miniGridIndices[idx][1];

                // Flag to check if a cell's possibility is repeated
                // in it's miniGrid
                bool isPossibilityRepeated = true;

                // At least 1 iteration should be completed to compute
                // possibility bitMap
                if (iterCount > 0)
                {
                    // Scan all bit values
                    for (int c = 1; c < 10; c++)
                    {
                        isPossibilityRepeated = false;

                        // Scan the bit set in possibility
                        if (IS_BIT_SET(c, grid[i][j].possibilities))
                        {
                            DEBUG_PRINT("Possibility bit %d set at [%d,%d]\n", c, i, j);

                            for (int a = 0; a < 3; a++)
                            {
                                for (int b = 0; b < 3; b++)
                                {
                                    // Search unknowns
                                    if (!IS_BIT_SET(0, grid[rS+a][cS+b].possibilities))
                                    {
                                        // Skip the cell being bit checked
                                        if ((i != (rS+a)) || (j != (cS+b)))
                                        {
                                            if (IS_BIT_SET(c, grid[rS+a][cS+b].possibilities))
                                            {
                                                DEBUG_PRINT("Possibility bit %d repeated at [%d,%d]\n", c, rS+a, cS+b);
                                                isPossibilityRepeated = true;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // Possibility bit matched the cell's value
                                        if (c == grid[rS+a][cS+b].value)
                                        {
                                            isPossibilityRepeated = true;
                                            break;
                                        }
                                    }
                                }

                                if (isPossibilityRepeated)
                                {
                                    break;
                                }
                            }

                            if (!isPossibilityRepeated)
                            {
                                DEBUG_PRINT("Setting cell value %d at [%d, %d]\n", c, i, j);

                                grid[i][j].value = c;
                                grid[i][j].possibilities = 1;
                                grid[i][j].prophecy = 1;
                                cellsToFill--;

                                ASSERT(grid[i][j].value == answer[i][j], "Incorrect value set\n")
                                break;
                            }
                        }
                    }
                }

                // Skip filled cells
                if (IS_BIT_SET(0, grid[i][j].possibilities))
                {
                    continue;
                }

                // BitMap to store the values which are set across row, col and miniGrid
                uint32_t elemBitMap = 0;

                // Check 3x3
                for (int a = 0; a < 3; a++)
                {
                    for (int b = 0; b < 3; b++)
                    {
                        if (grid[rS+a][cS+b].value != 0)
                        {
                            elemBitMap |= (1 << grid[rS+a][cS+b].value);
                        }
                    }
                }

                // Check row
                for (int a = 0; a < 9; a++)
                {
                    if (grid[i][a].value != 0)
                    {
                        elemBitMap |= (1 << grid[i][a].value);
                    }
                }

                // Check Col
                for (int b = 0; b < 9; b++)
                {
                    if (grid[b][j].value != 0)
                    {
                        elemBitMap |= (1 << grid[b][j].value);
                    }
                }

                // Clear pre-computed values
                grid[i][j].possibilities = 0;
                grid[i][j].prophecy = 0;

                // Compute bitMap
                grid[i][j].possibilities = ~elemBitMap & 0x3FE;

                Sudoku_UpdateCellBasedOnPossibilities(i, j);
            } // end of for
        }// end of for

        // Compute prophecy based on possibilities only if the previous logic
        // is not able to predict any further
        if ((prevCellsToFill == cellsToFill) && (iterCount > 0))
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    // Compute miniGrid indices
                    uint32_t idx = (j / 3) + (3 * (i / 3));
                    int rS = miniGridIndices[idx][0];
                    int cS = miniGridIndices[idx][1];

                    if ((iterCount > 0) && (grid[i][j].possibilities != 1))
                    {
                        // Check if a possibility bit is appearing only across 1 row or 1 col
                        for (int c = 1; c < 10; c++)
                        {
                            int32_t x = -1, y = -1;
                            bool isGuaranteed = true;

                            if (IS_BIT_SET(c, grid[i][j].possibilities))
                            {
                                bool isColMatchActive = false;
                                bool isRowMatchActive = false;

                                for (int a = 0; a < 3; a++)
                                {
                                    for (int b = 0; b < 3; b++)
                                    {
                                        // Check itself too
                                        if (IS_BIT_SET(c, grid[rS+a][cS+b].possibilities))
                                        {
                                            if ((x == -1) && (y == -1))
                                            {
                                                x = a;
                                                y = b;
                                            }
                                            else
                                            {
                                                if (x == a)
                                                {
                                                    if (isColMatchActive)
                                                    {
                                                        isGuaranteed = false;
                                                        break;
                                                    }

                                                    isRowMatchActive = true;
                                                }
                                                else if (y == b)
                                                {
                                                    if (isRowMatchActive)
                                                    {
                                                        isGuaranteed = false;
                                                        break;
                                                    }

                                                    isColMatchActive = true;
                                                }
                                                else
                                                {
                                                    // Not in a row or col
                                                    isGuaranteed = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    if (!isGuaranteed)
                                    {
                                        break;
                                    }
                                }

                                if (isGuaranteed)
                                {
                                    DEBUG_PRINT("Prophecy update bit %d at [%d,%d]\n", c, i, j);
                                    grid[i][j].prophecy |= (1 << c);
                                }
                            }
                        } // value loop
                    }// end prophecy
                } // for loop
            } // for loop
        } // stall check

        // Flag to check if any possibility is cleared
        bool isPossibilityCleared = false;

        // Reduce possibilities based on computed prophecy when stalled
        if ((prevCellsToFill == cellsToFill) && (iterCount > 0))
        {
            do
            {
                isPossibilityCleared = false;

                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 9; j++)
                    {
                        // Reduce possibilities based on prophecy
                        if (iterCount > 0)
                        {
                            uint32_t idx = (j / 3) + (3 * (i / 3));
                            int rS = miniGridIndices[idx][0];
                            int cS = miniGridIndices[idx][1];

                            for (int bit = 1; bit < 10; bit++)
                            {
                                int32_t _x = -1, _y = -1;
                                bool _isGuaranteed, _isBitCleared;

                                if (IS_BIT_SET(bit, grid[i][j].possibilities))
                                {
                                    _isGuaranteed = true;
                                    _isBitCleared = false;

                                    // Scan row
                                    for (int a = 0; a < 9; a++)
                                    {
                                        uint32_t idx = (a / 3) + (3 * (i / 3));

                                        int _rS = miniGridIndices[idx][0];
                                        int _cS = miniGridIndices[idx][1];

                                        // Skip elements in the same miniGrid
                                        if ((rS == _rS) && (cS == _cS))
                                        {
                                            continue;
                                        }

                                        // Check if prophecy has that bit set
                                        if (IS_BIT_SET(bit, grid[i][a].prophecy))
                                        {
                                            idx = (a / 3) + (3 * (i / 3));
                                            _rS = miniGridIndices[idx][0];
                                            _cS = miniGridIndices[idx][1];

                                            // Check if that bit is only set in the current row of it's mini grid
                                            for (int _a = 0; _a < 3; _a++)
                                            {
                                                for (int _b = 0; _b < 3; _b++)
                                                {
                                                    if (IS_BIT_SET(bit, grid[_a + _rS][_b + _cS].prophecy))
                                                    {
                                                        if ((_x == -1))
                                                        {
                                                            _x = _a;
                                                        }
                                                        else
                                                        {
                                                            if (_x != _a)
                                                            {
                                                                // Not in a row
                                                                _isGuaranteed = false;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                } // for _b

                                                if (!_isGuaranteed)
                                                {
                                                    break;
                                                }
                                            } // for _a

                                            if (_isGuaranteed)
                                            {
                                                // prophecy bit only exists in that row
                                                // clear bit
                                                DEBUG_PRINT("Clearing row, bit %d at [%d,%d]\n", bit, i, j);
                                                isPossibilityCleared = true;
                                                _isBitCleared = true;
                                                grid[i][j].possibilities &= ~(1 << bit);
                                                break;
                                            }
                                        } // prophecy bit check
                                    } // row loop

                                    if (_isBitCleared)
                                    {
                                        // Skip col check as it's already cleared
                                        DEBUG_PRINT("Skipping col check\n");
                                        continue;
                                    }

                                    // Scan col
                                    _isGuaranteed = true;

                                    for (int b = 0; b < 9; b++)
                                    {
                                        uint32_t idx = (j / 3) + (3 * (b / 3));

                                        int _rS = miniGridIndices[idx][0];
                                        int _cS = miniGridIndices[idx][1];

                                        // Skip elements in the same miniGrid
                                        if ((rS == _rS) && (cS == _cS))
                                        {
                                            continue;
                                        }

                                        // Check if prophecy has that bit set
                                        if (IS_BIT_SET(bit, grid[b][j].prophecy))
                                        {
                                            idx = (j / 3) + (3 * (b / 3));
                                            _rS = miniGridIndices[idx][0];
                                            _cS = miniGridIndices[idx][1];

                                            // Check if that bit is only set in the current col of it's miniGrid
                                            for (int _a = 0; _a < 3; _a++)
                                            {
                                                for (int _b = 0; _b < 3; _b++)
                                                {
                                                    if (IS_BIT_SET(bit, grid[_b + _rS][_a + _cS].prophecy))
                                                    {
                                                        if ((_y == -1))
                                                        {
                                                            _y = _a;
                                                        }
                                                        else
                                                        {
                                                            if (_y != _a)
                                                            {
                                                                // Not in a col
                                                                _isGuaranteed = false;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                } // for _b

                                                if (!_isGuaranteed)
                                                {
                                                    break;
                                                }
                                            } // for _a

                                            if (_isGuaranteed)
                                            {
                                                // prophecy bit only exists in that col
                                                // clear bit
                                                DEBUG_PRINT("Clearing col, bit %d at [%d,%d]\n", bit, i, j);
                                                isPossibilityCleared = true;
                                                grid[i][j].possibilities &= ~(1 << bit);
                                                break;
                                            }
                                        } // prophecy bit check
                                    } // col loop
                                } // possibilities bit check
                            } // bit loop
                        } // iter check

                        // If power of 2 possibility exist, set it's value
                        Sudoku_UpdateCellBasedOnPossibilities(i, j);

                        // Exit if a cell has been predicted
                        if (prevCellsToFill != cellsToFill)
                        {
                            break;
                        }
                    } // for

                    // Exit if a cell has been predicted
                    if (prevCellsToFill != cellsToFill)
                    {
                        break;
                    }
                } // for

                // Keep doing until cells are predicted
            } while ((isPossibilityCleared) && (prevCellsToFill == cellsToFill));
        } // if

        iterCount++;

    } //cellsToFill

    Sudoku_PrintGrid();
}
