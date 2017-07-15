#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "parse_http.h"




int start() {
	int sersock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(7801);
	saddr.sin_addr.s_addr = inet_addr("37.139.29.127");
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

	while (true){
		// begin to accept a connection
		int client = accept(sersock, (struct sockaddr*)&caddr, &size);
		printf("A client connected successfully\n");
		char buff[1024] = "";
		recv(client, &buff, 1023, 0);
		Map mmp = parse_request(buff);
		const char *method = get_request_method(&mmp);
		assert (strcmp(method, "GET") == 0);
		const char *destfile = get_dest_file(&mmp);
		char *res = (char *)calloc(sizeof(char), 60);
		assert (res != NULL);
		if (judge_file_exsit(destfile, res)) {
			printf("%s:File exists\n". destfile);
			write_to_client(client, res);
		}
		else {
			printf("%s:File not exists\n", destfile);
			//err_to_deal(client, res);
		}
		free(res);
		close(client);
	}
	return 0;
}

int main() {
	start();
	return 0;
}
