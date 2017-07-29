#include "scheduler.h"
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
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
	lfree(full_name);
	lfree(buf);
	close(fd);
}

bool connect_servers(Map *pmap) {
	//todo wait for json deal
	char *json_str = (char *)lalloc(512, 1);
	read_json_file("server.json", json_str);
	int server_num = -1;
	double tmp;


	light_value v;
	light_parse(&v, json_str);
	get_number(Value(&v, "server_num"), &tmp);
	server_num = (int)tmp;

	int i = 0;
	light_value *p = Value(&v, "server_lists");
	for (; i<server_num; ++i) {
		// get ip && port
		char *ip = (char *)lalloc(32, 1);
		int port = -1;
		double tmp;
		light_value *server = index_array(p, i);
		get_string(Value(server, "server_ip"), ip);
		get_number(Value(server, "server_port"), &tmp);
		port = (int)tmp;
		// need to save fds
		
		int cli = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = inet_addr(ip);

		int res = connect(cli, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
		if (res != 0) {
			perror("BIND SERVER ERROR");
		}

		// free mem
		lfree(ip);
	}
}
