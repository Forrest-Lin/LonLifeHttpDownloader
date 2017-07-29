#include "alloc.h"
int main () {
	void *p = NULL;
	p = lalloc(10, 2);
	printf("[%d]:[%p]\n", 10*2, p);
	p = lalloc(10, 2);
	printf("[%d]:[%p]\n", 10*2, p);
	lfree(p);


	char *str = (char *)lalloc(14, 1);
	strcpy(str, "hello, world");
	printf("%s\n", str);
	lfree(str);
	return 0;
}
