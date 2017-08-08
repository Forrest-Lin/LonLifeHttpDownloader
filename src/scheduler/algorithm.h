#ifndef _ALGO_H_
#define _ALGO_H_
#include <stdlib.h>
#include <string.h>
#include "../log/log.h"
#include "../map/map.h"
#include <stdio.h>
#include <time.h>


typedef struct Real_node{
	char *name;
	char *ip;
	int port;
	int fd;
}Real_node;

typedef struct FdSet{
	int * fd_array;
	int *task_nums;
	int fd_nums;
	int fd_fill_index;

	// consistent hash
	Map *rv_map;
	Map *real_message;
}FdSet;


//inner function//////////////////
// new a real node map item
Item *new_real_item(const char*key, Real_node *pnode);
// new real and virtaul map
Item *new_ra_item(const char *key, const char *value);
void init_fd_array(FdSet *, int);
void clear_fd_array(FdSet *);
void add_fd(FdSet *, Real_node *pnode);
int get_server_fd(FdSet *, const char *md5key);
int random_get(FdSet*);
int hash_get(FdSet *, const char *md5key);
int fd_index(FdSet *, int);
int fd_num(FdSet *);
bool in_range(FdSet *fd_set, int fd);
#endif
