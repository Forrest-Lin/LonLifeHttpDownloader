#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/*
 * 这里不实现一级空间配置器, 需要的地方直接使用malloc
 */

// all memory managements
typedef struct Heap{
	char *start;
	char *end;
	int total_size;
}Heap;

// 8 16 24 32 n*8->128
typedef struct chunk{
	struct chunk *next_chunk;
}chunk;



// main function
void *lalloc(int, int);
//alloc from chunk_list vs malloc
void *chunk_alloc(int);
// change size to n*8
int round_up(int);
// give a n*8 get n
int Index(int);
// refill mem for indx, if success return pointer, nor NULL
bool heap_refill(int);
