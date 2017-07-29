#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <assert.h>




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

	while (true){
		// begin to accept a connection
		int client = accept(sersock, (struct sockaddr*)&caddr, &size);
		printf("A client connected successfully\n");
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
