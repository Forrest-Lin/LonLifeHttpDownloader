#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>




void parse_request(const char *request) {
}
vod get_request_method(const char *request) {
}

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
		printf("Connect a client successfully\n");
		char buff[1024] = "";
		recv(client, &buff, 1023, 0);
		printf("%s\n", buff);
	}
	return 0;
}

int main() {
	start();
	return 0;
}
