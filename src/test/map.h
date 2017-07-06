#ifndef _MAP_H_
#define _MAP_H_
#include <string.h>

struct Item{
	char *key;
	char *value;
};
typedef struct Node Node;
struct Node{
	Item item;
	Node *next;
};
Node *map();
Node *new_node(const char*, const char *);
void del_node(Node *);
Node *value(const char *);
void del_map();

#endif
