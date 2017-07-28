#include "map.h"

/*
 * map基本的测试和使用办法
 *
 * */
Item *new_item(const char*key, void *value, int size) {
	Item *pres = (Item *)calloc(sizeof(Item), 1);
	pres->key = (char *)calloc(sizeof(char), strlen(key)+1);
	strcpy(pres->key, key);
	pres->value = calloc(sizeof(char), size);
	strcpy((char*)pres->value, value);
	return pres;
}

void inner_clear(void *p) {
	Item * pitem= (Item *)p;
	free(pitem->key);
	free(pitem->value);
	free(pitem);
}
void show_item(void *data) {
	Item *p = (Item *)data;
	printf("%s:%s\n", p->key, (char *)p->value);
}

int main () {
	Map mymap = map();
	Item *item = new_item("hgg", "80",3);
	add_item(&mymap, item);
	item = new_item("Mohican", "100", 4);
	add_item(&mymap, item);
	item = new_item("dgj", "70", 3);
	add_item(&mymap, item);
	item = new_item("jmt", "30", 3);
	add_item(&mymap, item);
	mapshow(&mymap, show_item);
	printf("%s\n", value(&mymap, "jmt"));
	map_clear(&mymap, inner_clear);
	return 0;
}
