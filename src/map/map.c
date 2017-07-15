#include "map.h"

int compare(void *first, void *second) {
	return strcmp(((Item *)first)->key, ((Item *)second)->key);
}


Map map() {
	Map res_map;
	res_map.tree = create_rb_tree();
	return res_map;
}

void add_item(Map *pmap, Item *item) {
	insert(pmap->tree, item, compare);
}

void mapshow(Map *pmap) {
	show(pmap->tree, show_item);
}

int value_compare(void *p, void *key) {
	return strcmp(((Item *)p)->key, key);
}
void *value(Map *pmap, char *key) {
	Node *p = find(pmap->tree, value_compare, key);
	if (p == NULL) {
		// not exsit
		printf("Not Exsit\n");
		return "";
	}
	return ((Item *)(p->data))->value;
}


void map_clear(Map *pmap) {
	clear(pmap->tree, inner_clear);
}
