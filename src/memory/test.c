#include "alloc.h"
int main () {
	void *p = NULL;
	int j = 0;
	int num = 100;
	for (; j<num; ++j) {
		int i = 1;
		for(; i<128; ++i) {
			p = lalloc(i, 1);
			printf("[%d]=>[%d]:[%p]\n", j, i, p);
			p = lalloc(i, 2);
			printf("[%d]=>[%d]:[%p]\n", j, i*2, p);
		}
	}
	sleep(200);
	return 0;
}
