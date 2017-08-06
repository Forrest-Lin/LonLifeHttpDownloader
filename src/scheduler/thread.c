#include "thread.h"

void set_no_blocking(int fd) {
	int flg = fcntl(fd, F_GETFL);
	int error = fcntl(fd, F_SETFL, flg|O_NONBLOCK);
	if (error == -1) {
		perror("FCNTL ERROR");
		LogFatal("Set nonblocking for fd failed");
	}
}
void *dealing_io(void *arg) {
	Argument *parg = (Argument *)arg;

	// using epoll
	int error = epoll_create(20);
	if (error == -1) {
		perror("EPOLL CREATE");
		LogFatal("Creat epoll fd failed");
	}
	int epollfd = error;
	const Max_epoll_size = 128;
	struct epoll_event *evnts = (struct epoll_event *)lalloc(sizeof(struct epoll_event), Max_epoll_size);

	//add server fd in epoll for EPOLLOUT event
	int n = fd_num(parg->fd_set);
	int i = 0;
	LogNotice("=>Adding server fds into epoll...");
	for (; i<n; ++i) {
		struct epoll_event tmp_evtns;
		tmp_evtns.events = EPOLLIN|EPOLLET;
		int new_fd = fd_index(parg->fd_set, i);
		tmp_evtns.data.fd = new_fd;
		error = epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &tmp_evtns);
		if (error == -1) {
			LogFatal("=>Add server fd into epoll failed");
		}
		set_no_blocking(new_fd);
	}
	while (true) {
		int new_fd = -1;
		pipe_read(parg->ppp, &new_fd);
		if (new_fd == -1) {
			continue;
		}
		LogNotice("=>Getted one fd from Pipe Ok");
		// create evnt types
		struct epoll_event evnt;
		evnt.events = EPOLLIN|EPOLLET;
		evnt.data.fd = new_fd;
		LogNotice("=>Adding new fd into epoll");
		error = epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &evnt);
		set_no_blocking(new_fd);
		if (error == -1) {
			perror("=>EPOLL_CTL_ADD ERROR");
			LogFatal("=>Add fd into epoll failed");
		}
		// start epoll wait
		LogNotice("=>Epoll wait for data arriving...");
		error = epoll_wait(epollfd, evnts, Max_epoll_size, -1);
		if (error == -1) {
			perror("=>EPOLL WAIT ERROR");
			LogFatal("=>Epoll wait failed");
		}

		int ready_num = error;
		// deal arrived fd
		int i = 0;
		for (; i<ready_num; ++i) {
			int ready_fd = evnts[i].data.fd;
			if (evnts[i].events & EPOLLIN) {
				if (in_range(parg->fd_set, ready_fd)) {
					// is server fd arriving
					//Stick package
					char *json_str = (char *)lalloc(sizeof(char), 510);
					char *buf = (char *)lalloc(sizeof(char), 30);
					int l = read(ready_fd, buf, 29);
					if (l == 0) {
						//todos
						LogFatal("=>Server closed, please check...");
					}
					else if (l < 0) {
						perror("=>READ ERROR");
						LogFatal("=>Read json data from server failed");
					}
					char *p = strstr(buf, "##");
					if(p == NULL) {
						LogWarning("=>Not get gap value \"##\" so skip this json package");
						continue;
					}

					strcat(json_str, p+2);
					int json_len = -1;
					sscanf(buf, "%d", &json_len);
					if (json_len == -1) {
						LogWarning("=>Not get json len so skip this json package");
						continue;
					}

					int left_size = json_len - (l - (p - buf + 2));
					LogNotice("=>Reading json str...");
					while (left_size != 0) {
						int readsize = 29;
						if (left_size < 29) {
							readsize = left_size;
						}

						memset(buf, 0, strlen(buf));
						l = read(ready_fd, buf, readsize);
						if (l == 0) {
							LogFatal("=>Server closed, please check it...");
						}
						else if (l < 0) {
							perror("=>READ ERROR");
							LogFatal("=>Read json str from server failed");
						}

						left_size -= l;
						strcat(json_str, buf);
					}
					lfree(buf);

					// json_str is on json_str
					LogNotice("=>Parsing json str...");
					LogNotice(json_str);
					light_value vp;
					light_parse(&vp, json_str);
					char *status_str = lalloc(sizeof(char), 4);
					char *header_str = lalloc(sizeof(char), 126);
					char *fd_str = lalloc(sizeof(char), 6);
					get_string(Value(&vp, "status"), status_str);
					get_string(Value(&vp, "header"), header_str);
					get_string(Value(&vp, "clientfd"), fd_str);

					int clientfd = -1;
					sscanf(fd_str, "%d", clientfd);
					//pass response header
					LogNotice("=>Sending header data to client...");
					write(clientfd, header_str, strlen(header_str));

					if (strcmp(status_str, "yes") == 0) {
						//pass file content
						LogNotice("=>File exsit, so send file...");
						char *filename = lalloc(sizeof(char), 30);
						get_string(Value(&vp, "filename"), filename);
						send_file(clientfd, filename);
						lfree(filename);
					}
					light_free(&vp);
					lfree(status_str);
					lfree(header_str);
					lfree(fd_str);
					lfree(json_str);

				}
				else {
					// is client request arriving
					char *readbuf = (char *)lalloc(sizeof(char), 30);
					char *request = (char *)lalloc(sizeof(char), 126);
					while (true) {
						// here is non blocking  so reading finish it
						memset(readbuf, 0, strlen(readbuf));
						int l = recv(ready_fd, readbuf, 30, 0);
						if (l < 0) {
							if (errno == EAGAIN || errno == EWOULDBLOCK) {
								LogWarning("=>Finish reading and begin to deal it...");
								break;
							}
							else {
								perror("=>RECV ERROR");
								LogFatal("=>Read data from client failed");
							}
						}	
						else if(l == 0) {
							LogWarning("=>One clinet is exit but not get full message");
							// remove from epoll
							error = epoll_ctl(epollfd, EPOLL_CTL_DEL, ready_fd, NULL);
							if (error == -1) {
								LogWarning("=>Del fd from epoll failed, please check...");
							}
							close(ready_fd);
							break;
						}
						// still have data to read
						strcat(request, readbuf);
					}
					lfree(readbuf);
					//
					// data is in request
					// choose one server
					int sfd = get_server_fd(parg->fd_set);

					LogNotice("=>Begin to pass data...");

					// add clien sign
					char addbuf[16] = "";
					sprintf(addbuf, "clientsign: %d", ready_fd);
					strcat(request, addbuf);
					char *sendbuf = (char *)lalloc(sizeof(char), strlen(request)+2+4);
					sprintf(sendbuf, "%d##%s", strlen(request), request);
					lfree(request);
					// send to server
					error = write(sfd, sendbuf, strlen(sendbuf));
					lfree(sendbuf);
					if (error == -1) {
						//todos
						LogFatal("=>Send data to server failed, please check...");
					}
				}
			}
			else {
				LogNotice("=>Event type unkonwn, so skip this event...");
				continue;
			}
		}
	}
	return NULL;
}

void getting_connect(int fd, Pipe *ppp) {
	struct sockaddr_in caddr;
	int size = sizeof(struct sockaddr_in);

	while (true) {
		// begin to accept a connection
		LogNotice("Waiting for client to connect...");
		int client = accept(fd, (struct sockaddr*)&caddr, &size);
		if (client == -1) {
			perror("ACCEPT ERROR");
			LogFatal("Get a client connet failed");
		}
		LogWarning("A client connected...");

		LogNotice("Sending fd into Pipe...");
		pipe_send(ppp, client);
	}
}
