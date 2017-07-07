#include "rbTree.h"
#include <stdio.h>
#include <stdbool.h>


int compare(void *first, void *second) {
	int res = *((int *)first) - *((int *)second);
	return res;
}
void myshow(void *data) {
	int p = *((int *)data);
	printf("%d\n", p);
}
int main() {
	RBTree *ptree = create_rb_tree();
	int array[100] = {};
	int i = 0;
	for (i=0; i<100; ++i) 
	{
		array[i] = rand();
	}

	for (i=0; i<100; ++i) {
		insert(ptree, &(array[i]), compare);
	}
	show(ptree, myshow);
	return 0;
}
