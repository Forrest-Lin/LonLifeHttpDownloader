#ifndef _ALGO_H_
#define _ALGO_H_
#include <stdlib.h>
#include <string.h>
#include "../log/log.h"
#include <stdio.h>
#include <time.h>


typedef struct FdSet{
	int * fd_array;
	int *task_nums;
	int fd_nums;
	int fd_fill_index;
}FdSet;

void init_fd_array(FdSet *, int);
void clear_fd_array(FdSet *);
void add_fd(FdSet *, int);
int get_server_fd(FdSet *);
int random_get(FdSet*);
/*
int robin_get();
int hash_get();
*/

#endif
