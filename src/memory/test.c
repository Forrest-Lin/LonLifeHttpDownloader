#include "alloc.h"
int main () {
	void *p = NULL;
	p = lalloc(10, 2);
	printf("[%d]:[%p]\n", 10*2, p);
	lfree(p, 10*2);
	p = lalloc(10, 2);
	printf("[%d]:[%p]\n", 10*2, p);
	return 0;
}
