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

void connect_servers() {
	// 8*13 = 104 -2 = 102
	LogNotice("Configuring schduler message...");
	char *json_str = (char *)lalloc(102, 1);
	read_json_file("schduler.json", json_str);
	LogNotice("Read json configure file");
		

	// get config
	light_value schduler_config;
	light_parse(&schduler_config, json_str);
	// get ip
	char *schduler_ip = (char *)lalloc(22, 1);
	get_string(Value(&schduler_config, "schduler_ip"), schduler_ip);
	// get port  because number is double so need tmp
	double tmp;
	get_number(Value(&schduler_config, "schduler_port"), &tmp);
	int schduler_port = (int)tmp;
	// get balance method //here todos
	char *method = (char *)lalloc(14, 1);
	get_string(Value(&schduler_config, "balance_method"), method);

	// bind schduler ip port and listen
	int sersock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(schduler_port);
	saddr.sin_addr.s_addr = inet_addr(schduler_ip);
	int err = bind(sersock, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	LogNotice("Binding socket address...");
	if (err == -1){
		LogFatal("Bind socket address failed");
	}
	lfree(json_str);

	//todo wait for json deal
	LogNotice("Configuring server message...");
	json_str = (char *)lalloc(512, 1);
	read_json_file("server.json", json_str);
	LogNotice("Read json configure file");
	int server_num = -1;


	light_value v;
	light_parse(&v, json_str);
	get_number(Value(&v, "server_num"), &tmp);
	server_num = (int)tmp;
	
	// schduler message
	FdSet *fd_set = (FdSet*)lalloc(sizeof(FdSet), 1);
	init_fd_array(fd_set, server_num);
	int i = 0;
	light_value *p = Value(&v, "server_lists");
	for (; i<server_num; ++i) {
		// get ip && port && name
		char *ip = (char *)lalloc(32, 1);
		char *name = (char *)lalloc(32, 1);
		int port = -1;
		double tmp;
		light_value *server = index_array(p, i);
		get_string(Value(server, "server_ip"), ip);
		get_number(Value(server, "server_port"), &tmp);
		get_string(Value(server, "server_name"), name);
		port = (int)tmp;
		// need to save fds
		
		int cli = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = inet_addr(ip);

		int res = connect(cli, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
		if (res != 0) {
			LogFatal("Connect server failed");
		}
		LogNotice("Connected server ok");

		// construct node and init it
		Real_node node = {name, ip, port, cli};
		add_fd(fd_set, &node);
		// free mem
		lfree(ip);
		lfree(name);
	}
	LogNotice("Connected all servers ok");
	lfree(json_str);


	// waiting for client
	//create listen queue
	err = listen(sersock, 5);
	if (err == -1){
		LogFatal("Listen api error");
	}
	// start mutil_thread
	const int thread_num = 3;
	pthread_t thread_array[3] = {};
	Pipe fdpipe;
	init_pipe(&fdpipe);
	set_no_blocking(fdpipe.fd[0]);

	i = 0;
	LogNotice("Creating threads...");
	// set thread arg
	Argument arg;
	arg.ppp = &fdpipe;
	arg.fd_set = fd_set;

	for (; i<thread_num; ++i) {
		err = pthread_create(thread_array+i, NULL, dealing_io, &arg);
		if (err == -1) {
			LogFatal("Create thread failed");
		}
		LogNotice("Created one thread ok");
	}

	// main thread
	getting_connect(sersock, &fdpipe);
}

void send_file(int fd, const char *filename) {
	// won't do check for file
	char *buf = (char *)lalloc(sizeof(char), 32);
	int filefd = open(filename, O_RDONLY);
	if (filefd == -1) {
		LogFatal("Open file failed");
	}
	
	while (read(filefd, buf, 31) != 0) {
		write(fd, buf, strlen(buf));
	}
	lfree(buf);
}
