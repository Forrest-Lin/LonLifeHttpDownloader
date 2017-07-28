#include "scheduler.h"
#include "../main/parse_http.h"

void build_push_string(const char *file_path, char *push_string) {
	// file path is like /doc/index.html
	assert (judge_file_exsit(file_path, NULL) == true);
	sprintf(push_string, "{\"type\":\"push\", \"object\":\"%s\"}", file_path);
}

void build_del_string(const char *file_path, char *del_string) {
	assert (judge_file_exsit(file_path, NULL) == true);
	sprintf(del_string, "{\"type\":\"del\", \"object\":\"%s\"}", file_path);
}

void build_get_string(const char *header, char *get_string) {
	// here have something todo
	//todos/////////////sign///
	sprintf(get_string, "{\"type\":\"get\", \"header\":\"%s\"}", header);
}

void read_json_file(const char *filename, char *str) {
	// json file comes from ../../etc
	char *full_name = (char *)lalloc(32, 1);
	strcpy(full_name, "../../etc/");
	strcat(full_name, filename);
	int fd = open(full_name, O_RDONLY);
	assert (fd != -1);

	char *buf = (char *)lalloc(32, 1);   
	while (read(fd, buf, 31)) {
		strcat(str, buf);
		memset(buf, 0, 32*1);
	}
	lfree(full_name, 32*1);
	lfree(buf, 32*1);
	close(fd);
}

bool connect_servers() {
	//todo wait for json deal
	char *json_str = (char *)lalloc(512, 1);
	read_json_file("server.json", json_str);
	int server_num = -1;
	double tmp;


	light_value v;
	light_parse(&v, json_str);
	get_number(Value(&v, "server_num"), &tmp);
	server_num = (int)tmp;

	// get ips
	
	char buf[32] = "";
	light_value *p = index_array(Value(&v, "server_lists"), 1);
	get_string(Value(p, "server_ip"), buf);
	printf("%s\n", buf);
	int i = 0;
	for (; i<server_num; ++i) {
	}
}
