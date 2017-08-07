#include "send_file.h"

void add_header(char *response, Map *pmap) {
	strcat(response, (const char *)value(pmap, "protype"));
	strcat(response, " ");
	strcat(response, (const char *)value(pmap, "status"));
	strcat(response, " ");
	strcat(response, (const char *)value(pmap, "mean"));
	strcat(response, "\r\n");


	strcat(response, "Server: ");
	strcat(response, (const char *)value(pmap, "Server"));
	strcat(response, "\r\n");


	strcat(response, "Content-Length: ");
	strcat(response, (const char *)value(pmap, "Content-Length"));
	strcat(response, "\r\n");

	strcat(response, "\r\n");
}


void get_header_msg(Map *pmap, int status, const char *mean, int length) {
	add_item(pmap, new_item("protype", "HTTP/1.1", 9));
	char buf[10] = "";
	sprintf(buf, "%d", status);
	add_item(pmap, new_item("status", buf, strlen(buf)));
	add_item(pmap, new_item("Server", "Simple Http Server", 19));
	add_item(pmap, new_item("mean", (void *)mean, strlen(mean)));
	memset(buf, 0, strlen(buf));
	sprintf(buf, "%d", length);
	add_item(pmap, new_item("Content-Length", buf, strlen(buf)));
	// if you want to add it just do like this
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
	char *response = (char *)lalloc(sizeof(char), 128);
	int total_len = get_file_size(file_name);
	get_header_msg(&header_map, 200, "OK", total_len);
	add_header(response, &header_map);

	// main message
	send (fd, response, strlen(response)+1, 0);
	//open file
	int filefd = open(file_name, O_RDONLY);
	assert (filefd != -1);
	int len = 0;
	while (len < total_len) {
		memset(response, 0, strlen(response));
		len += read(filefd, response, 127); 
		send(fd, response, strlen(response)+1, 0); 
	}
	close(filefd);
	lfree(response);
	map_clear(&header_map, clear_node);
}

Map init_status_map() {
	Map mmp = map();
	add_item(&mmp, new_item("200", "OK", 3));
	add_item(&mmp, new_item("400", "Bad Request", 12));
	add_item(&mmp, new_item("404", "Not Found", 10));
	add_item(&mmp, new_item("500", "Internal Server Error", 22));
	return mmp;
}

const char *get_status_mean(Map *pmap, int status) {
	char buf[4] = "";
	sprintf(buf, "%d", status);
	return (char *)value(pmap, buf);
}

void clear_status_map(Map *pmap) {
	map_clear(pmap, clear_node);
}

void compound_json(bool flg, const char *file_name, const char *response_header, const char *client_sign, char *json_str) {
	light_value *pobj = create_object();
	add_object(pobj, create_string("header"), create_string(response_header));
	add_object(pobj, create_string("clientsign"), create_string(client_sign));
	if (flg) {
		//sprintf(json_str, "{\"status\":\"yes\", \"filename\":\"%s\", \"header\":\"%s\"}", file_name, response_header);
		add_object(pobj, create_string("status"), create_string("yes"));
		add_object(pobj, create_string("filename"), create_string(file_name));
	}
	else {
		//sprintf(json_str, "{\"status\":\"no\", \"header\":\"%s\"}", response_header);
		add_object(pobj, create_string("status"), create_string("no"));
		show_value(pobj);
	}
	char *tmp = NULL;
	light_generate(pobj, &tmp, NULL);
	strcpy(json_str, tmp);
	light_free(pobj);
}
