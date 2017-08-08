#include "algorithm.h"

Item *new_real_item(const char*key, Real_node *pnode) {
	Item *pres = (Item *)lalloc(sizeof(Item), 1);
	pres->key = (char *)lalloc(sizeof(char), strlen(key)+1);
	strcpy(pres->key, key);
	pres->value = lalloc(sizeof(Real_node), 1);
	Real_node *p = (Real_node *)pres->value;
	p->name = (char *)lalloc(sizeof(char), strlen(pnode->name)+1);
	strcpy(p->name, pnode->name);
	p->ip = (char *)lalloc(sizeof(char), strlen(pnode->ip)+1);
	strcpy(p->ip, pnode->ip);

	p->port = pnode->port;
	p->fd = pnode->fd;
	return pres;
}

void clear_real_item(void *node) {
	Item *p = (Item *)node;
	Real_node *pnode = (Real_node *)p->value;
	lfree(pnode->ip);
	lfree(pnode->name);
	lfree(p->value);
	lfree(p->key);
	lfree(p);
}

void clear_ra_message(void *node) {
	Item *p = (Item *)node;
	lfree(p->key);
	lfree(p->value);
	lfree(p);
}

Item *new_ra_item(const char *key, const char *value) {
	Item *pres = (Item *)lalloc(sizeof(Item), 1);
	pres->key = (char *)lalloc(sizeof(char), strlen(key)+1);
	strcpy(pres->key, key);
	pres->value = lalloc(sizeof(char), strlen(value)+1);
	strcpy((char*)pres->value, value);
	return pres;
}

void init_fd_array(FdSet *fd_set, int size) {
	fd_set->fd_array = (int *)lalloc(sizeof(int), size);
	fd_set->fd_nums = size;
	fd_set->fd_fill_index = 0;
	fd_set->task_nums = (int *)lalloc(sizeof(int), size);

	// init map
	fd_set->rv_map = (Map *)lalloc(sizeof(Map), 1);
	fd_set->real_message = (Map *)lalloc(sizeof(Map), 1);
	*(fd_set->rv_map) = map();
	*(fd_set->real_message) = map();
}

void clear_fd_array(FdSet *fd_set) {
	lfree(fd_set->fd_array);
	lfree(fd_set->task_nums);
	fd_set->fd_array = NULL;
	fd_set->fd_nums = 0;
	fd_set->task_nums = NULL;
	fd_set->fd_fill_index = -1;

	// clear 2 map
	map_clear(fd_set->real_message, clear_real_item);
	map_clear(fd_set->rv_map, clear_ra_message);
}

void add_fd(FdSet *fd_set, Real_node *pnode) {
	if (fd_set->fd_fill_index >= fd_set->fd_nums) {
		LogFatal("Too many fd for fd_set");
	}
	(fd_set->fd_array)[(fd_set->fd_fill_index)++] = pnode->fd;

	// add one real node
	// deep copy
	
	char md5key[33] = "";
	char dest[50] = "";
	sprintf(dest, "%s:%d", pnode->ip, pnode->port);
	Compute_string_md5(dest, strlen(dest), md5key);
	Item *node_item = new_real_item(md5key, pnode);
	add_item(fd_set->real_message, node_item);

	int i = 0;
	int num = 5; // 5 virtual node
	for (; i<num; ++i) {
		strcat(dest, "1");
		// add virtual node
		char new_key[33] = "";
		Compute_string_md5(dest, strlen(dest), new_key);
		Item *rv_node = new_ra_item(md5key, new_key);
		add_item(fd_set->rv_map, rv_node);
	}
}

int fd_index(FdSet *fd_set, int i) {
	return fd_set->fd_array[i];
}

int fd_num(FdSet *fd_set) {
	return fd_set->fd_nums;
}

int get_server_fd(FdSet *fd_set, const char *key) {
	return random_get(fd_set);
}

int hash_get(FdSet *fd_set, const char *key) {
	// find first  higher than given key
	Node *p = fd_set->rv_map->tree->root;
	Node *tail = fd_set->rv_map->tree->tail;
	char *realnode_key = NULL;
	while (p != tail) {
		int res = strcmp(((Item *)(p->data))->key, key);
		if (res > 0) {
			char *realnode_key = (char *)(((Item *)(p->data))->value); 
			break;
		}
		else if (res < 0) {
			p = p->right;
		}
		else {
			LogFatal("Unbelieve happen, get same md5sum");
		}
	}
	//
	//return the root node msg : ring
	if (NULL == realnode_key) {
		realnode_key = (char *)(((Item *)(fd_set->rv_map->tree->root->data))->value);
	}
	Real_node *real_node = (Real_node *)value(fd_set->real_message, realnode_key);
	return real_node->fd;
}

bool in_range(FdSet *fd_set, int fd) {
	int i = 0;
	int num = fd_set->fd_nums;
	for (; i<num; ++i) {
		if (fd == fd_set->fd_array[i]) {
			return true;
		}
	}
	return false;
}

int random_get(FdSet *fd_set) {
	srand((unsigned)time(NULL));
	int indx = rand()%fd_set->fd_nums;
	++(fd_set->task_nums[indx]);
	return fd_set->fd_array[indx];
}
