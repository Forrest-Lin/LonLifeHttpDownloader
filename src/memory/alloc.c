#include "alloc.h"

void *lalloc(int size, int n){
	// behavior is like calloc
	assert (size > 0 && n > 0);
	int alloc_size = size*n;
	if (alloc_size > Max_size) {
		return calloc(size, n);
	}
	void *res = chunk_alloc(alloc_size);
	memset(res, 0, alloc_size);
	return res;
}


int round_up(int size) {
	return (size+Min_size-1) & ~(Min_size-1)
}

int index(int size) {
	assert(size%8 == 0);
	return size >> 3 - 1;
}

void *chunk_alloc(int size) {
	size = round_up(size);
	int indx = index(size);
	if (chunk_list[indx] != NULL) {
		chunk* res = chunk_list[indx];
		chunk_list[indx] = res->next_chunk;
		memset(res, 0, size);
		return res;
	}
	if (heap_refill(indx)) {
		//refill from heap successfully
		return chunk_alloc(size);
	}
	else {
		return NULL;
	}
}


bool heap_refill(int index) {
	int left_size = heap.end - heap.end;
	int size = Min_size * (index + 1);
	int need_size = size*Nnum;
	int num = Nnum;
	char *mem = NULL;
	//not enough for Nnums
	if (left_size < need_size) {
		// enough for one
		if (left_size > size) {
			num = left_size/size;
			mem = start;
			start += num*size;
		}
		else {
			// not enough for one so use malloc for new and deal left
			if (left_size > 0) {
				int indx = index(left_size);
				// head insert into chunk_list
				chunk *next = chunk_list[indx];
				chunk_list[indx] = (chunk*)start;
				chunk_list[indx].next_chunk = next;
				// start += left_size;

				int get_size = 2*need_size + round_up(heap.total_size>>1);
				heap.start = (char *)malloc(get_size);
				if (heap.start == NULL) {
					// malloc failed, so get some from behind
					int i = index(size) + 1;
					// start + add_sz == end
					int add_sz = size + Min_size;
					bool flg = false;
					for (; i<16; ++i, add_sz+=Min_size) {
						if (chunk_list[i] != NULL) {
							heap.start = (char *)chunk_list[i];
							chunk_list[i] = chunk_list[i]->next_chunk;
							heap.end += add_sz;
							flg = true;
							break;
						}
					}
					if (!flg) {
						return false;
					}
				}
				else {
					heap.end = start + get_size;
					heap.total_size += get_size;
				}
			}
		}
	}
	else {
		mem = start;
		start += need_size;
	}
	// link for chunk_list
	chunk *current = (chunk *)mem;
	chunk *next = current + 1;
	chunk_list[indx] = current;
	//1->2 2->3 3->4 19->20 next(20)->NULL
    for (int i=0; i<num-1; ++i) {
		current->next_chunk = next;

		current = next;
		++next;
	}	
	next->next_chunk = NULL;
	return true;
}
