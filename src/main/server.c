#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "process.h"




int main(int argc, char **argv) {
	assert (argc == 3);

	int port = -1;
	sscanf(argv[2], "%d", &port);
	multiprocess_start(argv[1], port);
	return 0;
}
