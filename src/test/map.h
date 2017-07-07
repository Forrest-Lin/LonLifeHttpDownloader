#ifndef _MAP_H_
#define _MAP_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rbTree.h"

struct Item{
	char *key;
	char *value;
};
typedef struct Item Item;
int compare(void *, void *);
void myshow(void *data);

struct Map{
	RBTree *tree;
};
typedef struct Map Map;

Map map();
Item *new_item(const char*, const char *);
void add_item(Map *pmap, Item *);
/*
void del_node(Node *);
*/
char *value(Map *, char *);
//void del_map();
void mapshow(Map *pmap);

#endif
