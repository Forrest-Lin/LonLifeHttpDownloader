#include "map.h"

int compare(void *first, void *second) {
	return strcmp(((Item *)first)->key, ((Item *)second)->key);
}
void myshow(void *data) {
	Item *p = (Item *)data;
	printf("%s:%s\n", p->key, p->value);
}


Map map() {
	Map res_map;
	res_map.tree = create_rb_tree();
	return res_map;
}

Item *new_item(const char*key, const char *value) {
	Item *pres = (Item *)calloc(sizeof(Item), 1);
	pres->key = (char *)calloc(sizeof(char), strlen(key)+1);
	strcpy(pres->key, key);
	pres->value = (char *)calloc(sizeof(char), strlen(value)+1);
	strcpy(pres->value, value);
	return pres;
}


void add_item(Map *pmap, Item *item) {
	insert(pmap->tree, item, compare);
}

void mapshow(Map *pmap) {
	show(pmap->tree, myshow);
}

int value_compare(void *p, void *key) {
	return strcmp(((Item *)p)->key, key);
}
char *value(Map *pmap, char *key) {
	Node *p = find(pmap->tree, value_compare, key);
	if (p == NULL) {
		// not exsit
		printf("Not Exsit\n");
		return "";
	}
	return ((Item *)(p->data))->value;
}
