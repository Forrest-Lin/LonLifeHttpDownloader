#include "map.h"

int main () {
	Map mymap = map();
	Item *item = new_item("hgg", "80");
	add_item(&mymap, item);
	item = new_item("Mohican", "100");
	add_item(&mymap, item);
	item = new_item("dgj", "70");
	add_item(&mymap, item);
	item = new_item("jmt", "30");
	add_item(&mymap, item);
	mapshow(&mymap);
	printf("%s\n", value(&mymap, "jmt"));
	return 0;
}
