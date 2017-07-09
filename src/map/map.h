#ifndef _MAP_H_
#define _MAP_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rbTree.h"

//定义基本的map一个元素的结构
struct Item{
	char *key;
	char *value;
};
typedef struct Item Item;

// compare函数是为了实现两个比较的
// myshow是展示其中一个元素的
// inner_clear是清除一个元素的内存
// 由于红黑树中存储的是void *所以在这里需要提供这么三个函数
int compare(void *, void *);
void myshow(void *data);
void inner_clear(void *);

struct Map{
	RBTree *tree;
};
typedef struct Map Map;

//构造一个map
Map map();
//构造一个元素key:value
Item *new_item(const char*, const char *);
//将元素加入map中
void add_item(Map *pmap, Item *);
//获取key对应value
char *value(Map *, char *);
//展示map中的数据
void mapshow(Map *pmap);
//清楚map所占用的内存
void map_clear(Map *);

#endif
