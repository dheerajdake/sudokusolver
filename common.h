#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>

// Enables debug prints
#define DEBUG                       (0)

// User defined
#define ASSERT(cond, ...)           if (!(cond)) {printf ("ASSERT: "__VA_ARGS__); assert(cond);}
#define PRINT_FILE_LINE()           printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__)

#if (DEBUG == 0)
#define DEBUG_PRINT(...)    NULL
#else
#define DEBUG_PRINT(...)    PRINT_FILE_LINE(); printf("\t\t"__VA_ARGS__)
#endif
