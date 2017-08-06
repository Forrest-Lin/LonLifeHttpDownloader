#include "process.h"

int create_sock(char *ip, int port) {
	// create socket and listen
	int sersock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	int err = bind(sersock, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	if (err == -1){
		perror("BIND ERROR");
		LogFatal("Bind api failed");
	}

	//create listen queue
	err = listen(sersock, 5);
	if (err == -1){
		perror("LISTEN ERROR");
		LogFatal("Listen api failed");
	}

	return sersock;
}

int get_client(int sersock) {
	struct sockaddr_in caddr;
	int size = sizeof(struct sockaddr_in);
	int client = accept(sersock, (struct sockaddr*)&caddr, &size);
	if (client < 0) {
		perror("ACCEPT ERROR");
		LogFatal("Accept api failed");
	}
	return client;
}

void deal_requests(int schduler, int msgid, struct producer_consumer *pc) {
	Map status_map = init_status_map();
	while (true) {
		// get one request from queue
		msgbuf item;
		lock_consume(pc);
		print_value(pc);
		LogNotice("=>Getting request from queue...");
		int error = msgrcv(msgid, &item, sizeof(item.mtext), 0, IPC_NOWAIT);
		if (error == -1) {
			perror("=>MSGRCV ERROR");
			LogFatal("=>Get request from queue failed");
		}
		unlock_consume(pc);


		int status_code = 200;
		// 1.parse http reequests
		LogNotice("=>Parsing http request...");
		Map mmp = parse_request(item.mtext);
		const char *method = get_request_method(&mmp);
		const char *client_sign = get_client_sign(&mmp);
		if (strcmp(method, "GET") != 0) {
			LogWarning("=>Client give not get method");
			status_code = 400;
		}

		//2.judge file
		const char *destfile = get_dest_file(&mmp);
		LogNotice("=>Judging whether file exsit...");
		char *res = (char *)lalloc(sizeof(char), 60);
		bool flg = judge_file_exsit(destfile, res);
		if (!flg) {
			LogWarning("=>File not exsit");
		}

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


		//3.compound http response
		Map response_map = map();
		// get file size
		int lenth = get_file_size(res);
		LogNotice("=>Compounding http response header...");
		get_header_msg(&response_map, status_code, 
				get_status_mean(&status_map, status_code), lenth);
		char *response = (char *)lalloc(sizeof(char), 254);
		add_header(response, &response_map);
		map_clear(&response_map, clear_node);

		//4.compound json message
		char *json_res = (char *)lalloc(sizeof(char), 510);
		LogNotice("=>Compounding json message...");
		compound_json(flg, res, response, client_sign, json_res);
		lfree(res);
		lfree(response);
		map_clear(&mmp, clear_node);

		// pass json_res to schduler
		//Stick package
		int buflen = strlen(json_res) + 2 + 3 + 1;
		char *sendbuf = (char *)lalloc(sizeof(char), buflen);
		sprintf(sendbuf, "%d##%s", strlen(json_res), json_res);
		LogNotice("=>Sending data to scheduler...");
		error = write(schduler, sendbuf, buflen);
		if (error == -1) {
			LogFatal("=>Sending data fialed");
		}
		lfree(json_res);
		lfree(sendbuf);

	}
}

void create_reactor(int schduler, int msgid, struct producer_consumer *pc) {
	char *readbuf = (char *)lalloc(sizeof(char), 30);
	int len = -1;
	int request_size = 0;
	//Stick package
	while (true) {
		len = read(schduler, readbuf, 29);
		if (len == 0) {
			LogFatal("Scheduler closed the connection");
			//todo
		}
		else if (len < 0) {
			perror("CREATE REATOR READ error");
			LogFatal("READ api filed");
		}

		//Stick package gap as ##
		char *p = strstr(readbuf, "##");
		if (p == NULL) {
			LogWarning("Not get gap value \"##\" so skip this package");
			continue;
		}

		request_size = -1;
		sscanf(readbuf, "%d", &request_size);
		if (request_size == -1) {
			LogWarning("Cannot get request length, so skip this package");
			continue;
		}

		// left size equal all request size sub this package size
		int left_size = request_size - (len - (p - readbuf + 2));
		LogNotice("Reading a request package...");

		//init msg item
		msgbuf item;
		memset(item.mtext, 0, sizeof(item.mtext));
		item.mtype = 1;

		// get item data
		strcat(item.mtext, p+2);
		while (left_size != 0) {
			// confirm not read more
			int readsize = 29;
			if (left_size < 29) {
				readsize = left_size;
			}
			memset(readbuf, 0, strlen(readbuf));
			len = read(schduler, readbuf, readsize);
			if (len == 0) {
				LogFatal("Schduler closed the connection but reading unfinish");
				//todos
			}	
			else if (len < 0) {
				perror("READ ERROR");
				LogFatal("Read http header filed");
			}

			// mov left_size
			left_size -= len;
			strcat(item.mtext, readbuf);
		}

		// finish one, so put it into msg queue
		LogNotice("Sending data into message queue...");
		// p
		print_value(pc);
		lock_produce(pc);
		print_value(pc);
		int error = msgsnd(msgid, &item, sizeof(item.mtext), IPC_NOWAIT);
		if (error == -1) {
			perror("MSGSEND ERROR");
			LogFatal("Send message into queue failed");
		}
		print_value(pc);
		unlock_produce(pc);
		print_value(pc);
		//v
	}
}
void multiprocess_start(char *ip, int port) {
	const int process_num = 4;
	LogNotice("Starting process pool...");
	int process_fds[4] = {};

	// message queue
	LogNotice("Creating message queue...");
	int msgid = msgget((key_t)port, IPC_CREAT|IPC_EXCL|0644);
	if (msgid == -1) {
		perror("MSGGET ERROR");
		LogFatal("Create message queue failed");
	}
	// producer consumer
	struct producer_consumer mutx;
	LogNotice("Creating message mutex...");
	LogNotice("Initing mutex...");
	init_pc(&mutx, port, 10);

	LogNotice("Creating server socket...");
	int sersock = create_sock(ip, port);

	// get schduler link
	LogNotice("Getting schduler connecting...");
	int schduler = get_client(sersock);

	int i = 1;
	for (; i<process_num; ++i) {
		process_fds[i-1] = fork();
		if (process_fds[i-1] == 0) {
			// child 
			LogNotice("=>Started one processed...");
			deal_requests(schduler, msgid, &mutx);
			break;
		}
		else {
			continue;
		}
	}
	LogNotice("Starting parent process...");

	// parent process do
	// read from schduler, and push into message queue
	LogNotice("Starting main loop for reading data...");
	// producer consumer
	create_reactor(schduler, msgid, &mutx);

	// if over then wait child process
	i = 0;
	for (; i<process_num-1; ++i) {
		int stat = 0;
		int cpid = wait(&stat);
		printf("[%d]:[%d]\n", cpid, stat);
	}
}

