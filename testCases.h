#include<stdint.h>

#define TEST_CASE 1

// Grid easy
#if (TEST_CASE == 0)
uint32_t suduko[9][9] =
{
	{4, 0, 7,  8, 0, 5,  0, 0, 9},
	{0, 0, 3,  7, 0, 1,  0, 5, 0},
	{0, 1, 0,  0, 4, 0,  7, 8, 6},
	
	{1, 0, 2,  0, 0, 0,  0, 7, 4},
	{0, 9, 8,  4, 3, 0,  0, 0, 0},
	{0, 6, 0,  9, 1, 0,  8, 0, 5},

	{0, 0, 0,  0, 0, 0,  0, 4, 0},
	{3, 0, 9,  0, 0, 8,  6, 0, 0},
	{2, 0, 0,  3, 0, 4,  1, 9, 8}
};

uint32_t answer[9][9] = 
{
	{4, 2, 7,  8, 6, 5,  3, 1, 9},
	{6, 8, 3,  7, 9, 1,  4, 5, 2},
	{9, 1, 5,  2, 4, 3,  7, 8, 6},
	
	{1, 3, 2,  5, 8, 6,  9, 7, 4},
	{5, 9, 8,  4, 3, 7,  2, 6, 1},
	{7, 6, 4,  9, 1, 2,  8, 3, 5},

	{8, 7, 1,  6, 2, 9,  5, 4, 3},
	{3, 4, 9,  1, 5, 8,  6, 2, 7},
	{2, 5, 6,  3, 7, 4,  1, 9, 8}
};
#elif (TEST_CASE == 1)
uint32_t suduko[9][9] =
{
	{0, 0, 0,  0, 0, 0,  0, 0, 2},
	{0, 0, 0,  0, 0, 0,  9, 4, 0},
	{0, 0, 3,  0, 0, 0,  0, 0, 5},
	
	{0, 9, 2,  3, 0, 5,  0, 7, 4},
	{8, 4, 0,  0, 0, 0,  0, 0, 0},
	{0, 6, 7,  0, 9, 8,  0, 0, 0},

	{0, 0, 0,  7, 0, 6,  0, 0, 0},
	{0, 0, 0,  9, 0, 0,  0, 2, 0},
	{4, 0, 8,  5, 0, 0,  3, 6, 0}
};


uint32_t answer[9][9] =
{
	{6, 8, 4,  1, 5, 9,  7, 3, 2},
	{7, 5, 1,  8, 3, 2,  9, 4, 6},
	{9, 2, 3,  6, 7, 4,  1, 8, 5},
	
	{1, 9, 2,  3, 6, 5,  8, 7, 4},
	{8, 4, 5,  2, 1, 7,  6, 9, 3},
	{3, 6, 7,  4, 9, 8,  2, 5, 1},

	{2, 3, 9,  7, 4, 6,  5, 1, 8},
	{5, 1, 6,  9, 8, 3,  4, 2, 7},
	{4, 7, 8,  5, 2, 1,  3, 6, 9}
};
#else
uint32_t suduko[9][9] =
{
	{0, 0, 0,  0, 0, 0,  6, 8, 0}, // 1 7 2  5 4 9  6 8 3
	{0, 0, 0,  0, 7, 3,  0, 0, 9}, // 6 4 5  8 7 3  2 1 9
	{3, 0, 9,  0, 0, 0,  0, 4, 5}, // 3 8 9  2 6 1  7 4 5
	
	{4, 9, 0,  0, 0, 0,  0, 0, 0}, // 4 9 6  3 2 7  8 5 1
	{8, 0, 3,  0, 5, 0,  9, 0, 2}, // 8 1 3  4 5 6  9 7 2
	{0, 0, 0,  0, 0, 0,  0, 3, 6}, // 2 5 7  1 9 8  4 3 6

	{9, 6, 0,  0, 0, 0,  3, 0, 8}, // 9 6 4  7 1 5  3 2 8
	{7, 0, 0,  6, 8, 0,  0, 0, 0}, // 7 3 1  6 8 2  5 9 4
	{0, 2, 8,  0, 0, 0,  0, 0, 0}  // 5 2 8  9 3 4  1 6 7
};

uint32_t answer[9][9] = 
{
	{1, 7, 2,  5, 4, 9,  6, 8, 3},
	{6, 4, 5,  8, 7, 3,  2, 1, 9},
	{3, 8, 9,  2, 6, 1,  7, 4, 5},
	
	{4, 9, 6,  3, 2, 7,  8, 5, 1},
	{8, 1, 3,  4, 5, 6,  9, 7, 2},
	{2, 5, 7,  1, 9, 8,  4, 3, 6},

	{9, 6, 4,  7, 1, 5,  3, 2, 8},
	{7, 3, 1,  6, 8, 2,  5, 9, 4},
	{5, 2, 8,  9, 3, 4,  1, 6, 7}
};
#endif