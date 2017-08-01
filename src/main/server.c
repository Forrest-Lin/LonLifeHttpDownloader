#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "parse_http.h"
#include "send_file.h"




int start(const char *ip, int port) {
	int sersock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	int err = bind(sersock, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	if (err == -1){
		perror("BIND ERROR");
		return  err;
	}

	//create listen queue
	err = listen(sersock, 5);
	if (err == -1){
		perror("LISTEN ERROR");
		return err;
	}

	struct sockaddr_in caddr;
	int size = sizeof(struct sockaddr_in);
	Map status_map = init_status_map();

	while (true){
		// begin to accept a connection
		int client = accept(sersock, (struct sockaddr*)&caddr, &size);
		printf("A client connected successfully\n");
		char buff[1024] = "";
		recv(client, &buff, 1023, 0);
		int status_code = 200;

		// 1.parse http reequests
		Map mmp = parse_request(buff);
		const char *method = get_request_method(&mmp);
		if (strcmp(method, "GET") != 0) {
			LogWarning("Client give not get method");
			status_code = 400;
		}

		//2.judge file
		const char *destfile = get_dest_file(&mmp);
		char *res = (char *)lalloc(sizeof(char), 60);
		bool flg = judge_file_exsit(destfile, res);
		

		// if status_code has changed just set flg 
		// if status_code unchanged and file unexsit just set status_code 404
		if (status_code == 200) {
			if (!flg) {
				status_code = 404;
			}
		}
		else {
			flg = false;
		}
		

		//compound http response
		Map response_map = map();
		// get file size
		int lenth = get_file_size(res);
		get_header_msg(&response_map, status_code, 
				get_status_mean(&status_map, status_code), lenth);
		char *response = (char *)lalloc(sizeof(char), 254);
		add_header(response, &response_map);
		map_clear(&response_map, clear_node);

		//compound json message
		char *json_res = (char *)lalloc(sizeof(char), 510);
		compound_json(flg, res, response, json_res);
		printf("%s\n", json_res);

		lfree(response);

		// pass json_res to schduler
		write(client, json_res, strlen(json_res));

		lfree(json_res);
		lfree(res);
		close(client);
	}
	return 0;
}

int main(int argc, char **argv) {
	assert (argc == 3);

	int port = -1;
	sscanf(argv[2], "%d", &port);
	start(argv[1], port);
	return 0;
}
