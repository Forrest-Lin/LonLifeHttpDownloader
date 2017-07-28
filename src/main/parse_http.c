#include "parse_http.h"

Item *new_item(const char*key, void *value, int size) {
	Item *pres = (Item *)calloc(sizeof(Item), 1);
	pres->key = (char *)calloc(sizeof(char), strlen(key)+1);
	strcpy(pres->key, key);
	pres->value = calloc(sizeof(char), size);
	strcpy((char*)pres->value, value);
	return pres;
}

void clear_node(void *p) {
	Item * pitem= (Item *)p;
	free(pitem->key);
	free(pitem->value);
	free(pitem);
}
void show_node(void *data) {
	Item *p = (Item *)data;
	printf("%s:%s\n", p->key, (char *)p->value);
}
Map parse_request(const char *request) {
	Map res_map = map();
	char *tmp = (char *)calloc(sizeof(char), strlen(request)+1);
	assert (tmp != NULL);
	strcpy(tmp, request);

	char *p = strtok(tmp, "\r\n");
	// add method name
	char *q = strchr(p, ' ');
	char *buf = (char *)calloc(sizeof(char), q-p+1);
	strncpy(buf, p, q-p);
	add_item(&res_map, new_item("method", buf, strlen(buf)+1));
	free(buf);


	//add dest name
	p = q + 1;
	q = strchr(q+1, ' ');
	buf = (char *)calloc(sizeof(char), q-p+1);
	strncpy(buf, p, q-p);
	add_item(&res_map, new_item("destfile", buf, strlen(buf)+1));
	free(buf);

	//add protocol type
	q = q + 1;
	buf = (char *)calloc(sizeof(char), strlen(q)+1);
	strncpy(buf, q, strlen(q));
	add_item(&res_map, new_item("protype", buf, strlen(buf)+1));
	free(buf);

	while ((p = strtok(NULL, "\r\n")) != NULL) {
		//add other msg consiss of ':'
		q = strchr(p, ':');
		char *key = (char *)calloc(sizeof(char), q-p+1);
		char *value = (char *)calloc(sizeof(char), strlen(q)-1);
		assert (key != NULL && value != NULL);
		strncpy(key, p, q-p);
		strcpy(value, q+2);
		add_item(&res_map, new_item(key, value, strlen(q)-1));
		free(key);
		free(value);
	}
	return res_map;
}

const char *get_request_method(Map *pmap) {
	return (const char *)value(pmap, "method");
}

bool judge_file_exsit(const char *file_name, char *res_buf) {
	char res[128] = "";
	const char *root = "../../docroot";
	strcpy(res, root);
	strcat(res, file_name);
	if (strcmp(file_name, "/") == 0) {
		strcat(res, "default.txt");
	}
	if (res_buf != NULL) {
		strcpy(res_buf, res);
	}
	return access(res, F_OK|R_OK) != -1;
}


const char *get_dest_file(Map *pmap) {
	return (const char *)value(pmap, "destfile");
}
