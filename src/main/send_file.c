#include "send_file.h"

void add_header(char *response, Map *pmap) {
	sprintf(response, "HTTP/1.1 %d %s\r\n", status, mean);
	strcat(response, "Server: Simple Httpserver\r\n");
}


void get_header_msg(Map *pmap, int status, const char *mean, int length) {
	add_item(&header_map, new_item("protype", "HTTP/1.1"));
	char buf[10] = "";
	sprintf(buf, "%d", status);
	add_item(&header_map, new_item("status", buf));
	add_item(&header_map, new_item("Server", "Simple Http Server"));
	add_item(&header_map, new_item("mean", mean));
	memset(buf, 0, strlen(buf));
	sprintf(buf, "%d", length);
	add_item(&header_map, new_item("Content-Length", buf));
	// if you want to add it just do like this
	free(buf);
}

int get_file_size(const char *file_name) {
	struct stat statbuf;
	if (stat(file_name, &statbuf) < 0) {
		perror("STAT FILE");
		return 0;
	}
	else {
		return statbuf.st_size;
	}
}

void write_to_client(int fd, const char *file_name) {
	Map header_map = map();
	char *response = (char *)calloc(sizeof(char), 128);
	get_header_msg(pmap, 200, "OK", get_file_size(file_name));
	add_header(response, pmap);
}
