#include "algorithm.h"

void init_fd_array(FdSet *fd_set, int size) {
	fd_set->fd_array = (int *)lalloc(sizeof(int), size);
	fd_set->fd_nums = size;
	fd_set->fd_fill_index = 0;
	fd_set->task_nums = (int *)lalloc(sizeof(int), size);
}

void clear_fd_array(FdSet *fd_set) {
	lfree(fd_set->fd_array);
	lfree(fd_set->task_nums);
	fd_set->fd_array = NULL;
	fd_set->fd_nums = 0;
	fd_set->task_nums = NULL;
	fd_set->fd_fill_index = -1;
}

void add_fd(FdSet *fd_set, int fd) {
	if (fd_set->fd_fill_index >= fd_set->fd_nums) {
		LogFatal("Too many fd for fd_set");
	}
	(fd_set->fd_array)[(fd_set->fd_fill_index)++] = fd;
}

int get_server_fd(FdSet *fd_set) {
	return random_get(fd_set);
}

int random_get(FdSet *fd_set) {
	srand((unsigned)time(NULL));
	int indx = rand()%fd_set->fd_nums;
	++(fd_set->task_nums[indx]);
	return fd_set->fd_array[indx];
}
