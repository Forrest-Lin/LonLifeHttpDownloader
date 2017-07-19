#include <stdio.h>
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

// main manage var
Heap heap = {NULL, NULL, 0};
chunk *chunk_list[16] = {NULL};
int Min_size = 8;
int Max_size = 128;
int Nnum = 20;


// main function
void *lalloc(int, int);
//alloc from chunk_list vs malloc
void *chunk_alloc(int);
// change size to n*8
int round_up(int);
// give a n*8 get n
int index(int);
// refill mem for indx, if success return pointer, nor NULL
bool heap_refill(int);
