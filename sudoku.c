#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include "testCases.h"

#define IS_BIT_SET(bit, num)    ((num >> bit) & 1)
#define IS_POWER_OF_TWO(num)    (!(num & (num-1)))

typedef struct _cell
{
	uint32_t value;
	uint32_t possibilities;
	uint32_t prophecy;
} cell_t;

// Check index
uint32_t miniGridIndices[9][2] =
{
	{0, 0}, {0, 3}, {0, 6},
	{3, 0}, {3, 3}, {3, 6},
	{6, 0}, {6, 3}, {6, 6}
};

cell_t grid[9][9];

void printGrid()
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

int main()
{
	uint32_t cellsToFill = 0;
	memset(grid, 0, sizeof(cell_t) * 9 * 9);

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

	// Guess grid
	int iterCount = 0;
	printf("CellsToFill %d\n", cellsToFill);

	uint32_t prevCellsToFill;

	while (cellsToFill != 0)
	{
		prevCellsToFill = cellsToFill;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (IS_BIT_SET(0, grid[i][j].possibilities))
				{
					continue;
				}

				// miniGrid details
				uint32_t idx = (j / 3) + (3 * (i / 3));
				int rS = miniGridIndices[idx][0];
				int cS = miniGridIndices[idx][1];

				bool isPossibilityRepeated = true;

				if (iterCount > 0)
				{
					// Check for multiple number occurances
					// in mini grid
					for (int c = 1; c < 10; c++)
					{
						isPossibilityRepeated = false;

						if (IS_BIT_SET(c, grid[i][j].possibilities))
						{
							//printf("[%d,%d] checking bit %d\n", i, j, c);
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
												//printf(">>> [%d,%d]\n", rS+a, cS+b);
												isPossibilityRepeated = true;
												break;
											}
										}
									}
									else
									{
										// known value == bit value
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
								printf("BIT ELIMINATION [%d, %d] = %d || %d\n", i, j, c, cellsToFill);
								grid[i][j].value = c;
								grid[i][j].possibilities = 1;
								grid[i][j].prophecy = 1;
								cellsToFill--;
								//printGrid();


								if (grid[i][j].value != answer[i][j])
								{
									printf("Wrong value predicted\n");
									assert(false);
								}
								break;
							}
						}
					}
				}

				if (IS_BIT_SET(0, grid[i][j].possibilities))
				{
					continue;
				}

				// Element bitMap
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

				// Clear possibilities
				grid[i][j].possibilities = 0;
				grid[i][j].prophecy = 0;

				// Sets bits for missing elements
				for (int k = 1; k < 10; k++)
				{
					if (!IS_BIT_SET(k, elemBitMap))
					{
						grid[i][j].possibilities |= (1 << k);
					}
				}

				if (IS_POWER_OF_TWO(grid[i][j].possibilities))
				{
					// Find the bit set
					int l = 1;
					for (; l < 10; l++)
					{
						if ((grid[i][j].possibilities >> l) & 1)
						{
							grid[i][j].value = l;
							grid[i][j].possibilities = 1;
							grid[i][j].prophecy = 1;
							cellsToFill--;
							printf("POWER OF TWO possibility [%d, %d] = %d\n", i, j, l);
							//printGrid();

							if (grid[i][j].value != answer[i][j])
							{
								printf("Wrong value predicted\n");
								assert(false);
							}

							break;
						}
					}
				}
			} // end of for
		}// end of for

#if 1
		// Compute prophecy based on possibilities
		if ((prevCellsToFill == cellsToFill) && (iterCount > 0))
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					uint32_t idx = (j / 3) + (3 * (i / 3));
					int rS = miniGridIndices[idx][0];
					int cS = miniGridIndices[idx][1];

					if ((iterCount > 0) && (grid[i][j].possibilities != 1))
					{
						// Check if a possibility elem is appearing only across 1 row or 1 col
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
									//printf("updating prophecy [%d,%d] = %d\n", i, j, c);
									grid[i][j].prophecy |= (1 << c);
									//printGrid();
								}
								else
								{
									//printGrid();
									//printf("failed prophecy [%d,%d] = %d\n", i, j, c);
								}
							}
						} // value loop
					}// end prophecy
				}
			}
		}

		// Reduce possibilities based on computed prophecy
		bool myCustomFlag = false;
		if ((prevCellsToFill == cellsToFill) && (iterCount > 0))
		{   do{ myCustomFlag = false; //printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n"); //printGrid(); 
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

						//printf("@@@ [%d, %d] = [%d, %d]\n", i, j, rS, cS);

						for (int bit = 1; bit < 10; bit++)
						{
							int32_t _x = -1, _y = -1;
							bool _isGuaranteed, _isBitCleared;

							if (IS_BIT_SET(bit, grid[i][j].possibilities))
							{
								_isGuaranteed = true;
								_isBitCleared = false;

								// Scan row
								//printf("__scanning row for bit %d\n", bit);
								for (int a = 0; a < 9; a++)
								{
									uint32_t idx = (a / 3) + (3 * (i / 3));

									int _rS = miniGridIndices[idx][0];
									int _cS = miniGridIndices[idx][1];

									// Skip elements in the same miniGrid
									if ((rS == _rS) && (cS == _cS))
									{
										//printf("__Skipping row [%d,%d]\n", i, a);
										continue;
									}
									
									// Check if prophecy has that bit set
									if (IS_BIT_SET(bit, grid[i][a].prophecy))
									{
										//printf("__validating row prophecy bit %d\n", bit);
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
											// reduce possibilities
											printf("PP row clearing %d at [%d,%d]\n", bit, i, j);
											myCustomFlag = true;
											_isBitCleared = true;
											//printGrid();
											grid[i][j].possibilities &= ~(1 << bit);
											break;
										}
									} // prophecy bit check
								} // row loop

								if (_isBitCleared)
								{
									// Skip col check as it's already cleared
									//printf("Skipping col check\n");
									continue;
								}

								// Scan col
								//printf("__scanning col for bit %d\n", bit);
								_isGuaranteed = true;
								for (int b = 0; b < 9; b++)
								{
									uint32_t idx = (j / 3) + (3 * (b / 3));

									int _rS = miniGridIndices[idx][0];
									int _cS = miniGridIndices[idx][1];

									// Skip elements in the same miniGrid
									if ((rS == _rS) && (cS == _cS))
									{
										//printf("__Skipping col [%d,%d]\n", b, j);
										continue;
									}
									
									// Check if prophecy has that bit set
									if (IS_BIT_SET(bit, grid[b][j].prophecy))
									{
										//printf("__validating col prophecy bit %d\n", bit);
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
											// prophecy bit only exists in that row
											// reduce possibilities
											printf("PP col clearing %d at [%d,%d]\n", bit, i, j);
											myCustomFlag = true;
											//printGrid();
											grid[i][j].possibilities &= ~(1 << bit);
											break;
										}
									} // prophecy bit check
								} // col loop
							} // possibilities bit check
						} // bit loop
					} // iter check


					///////////////////////////
					// If power of 2 possibility exist, set it's value
					if (IS_POWER_OF_TWO(grid[i][j].possibilities))
					{
						// Find the bit set
						int l = 1;
						for (; l < 10; l++)
						{
							if ((grid[i][j].possibilities >> l) & 1)
							{
								grid[i][j].value = l;
								grid[i][j].possibilities = 1;
								grid[i][j].prophecy = 1;
								cellsToFill--;
								printf("__POWER OF TWO possibility [%d, %d] = %d\n", i, j, l);
								//printGrid();

								if (grid[i][j].value != answer[i][j])
								{
									printf("__Wrong value predicted\n");
									assert(false);
								}

								break;
							}
						}
					}

					if (prevCellsToFill != cellsToFill)
					{
						break;
					}
				}// for

				if (prevCellsToFill != cellsToFill)
				{
					break;
				}
			}// for

			}while((myCustomFlag) && (prevCellsToFill == cellsToFill));
		}// if
#endif
		iterCount++;
		//printf("iterCount %d %d %d\n", cellsToFill, prevCellsToFill, iterCount);
		//printGrid();
	}//cellsToFill

	// Print grid
	printGrid();
	printf("Done\n");

	return 0;
}
