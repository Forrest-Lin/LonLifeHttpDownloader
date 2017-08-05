#include "pipe.h"

void init_pipe(Pipe *ppp) {
	int error = pipe(ppp->fd);
	if (error == -1) {
		LogFatal("Init pipe for thread communication failed");
	}
	//
	// init mutex
	error = pthread_mutex_init(&ppp->mutx, NULL);
	if (error == -1) {
		LogFatal("Init mutex failed");
	}
}

void pip_send(Pipe *ppp, int fd) {
	int error = pthread_mutx_lock(&ppp->mutx);
	if (error == -1) {
		perror("Lock");
		LogFatal("Try to lock mutex when send fd failed");
	}

	char fdbuf[7] = "";
	sprintf(fdbuf, "%d#", fd);
	error = send(ppp->fd[1], fdbuf, strlen(fdbuf), 0);
	if (error == -1) {
		LogFatal("Send fd into Pipe failed");
	}

	error = pthread_mutx_unlock(&ppp->mutx);
	if(error == -1) {
		perror("Unlock");
		LogFatal("Try to unlock mutex when send fd failed");
	}
}


void pipe_read(Pipe *ppp, int *pfd) {
	int error = pthread_mutx_lock(&ppp->mutx);
	if (error == -1) {
		perror("Lock");
		LogFatal("Try to lock mutex when recv fd failed");
	}

	char *rcvbuf[7] = "";
	error = recv(ppp->fd[0], rcvbuf, 6, MSG_PEEK);
	if (error == -1 && errno != EAGAIN) {
		perror("Recv");
		LogFatal("Recv fd from Pipe failed with PEEK flg");
	}
	else if (error != -1) {
		char *p = strchr(rcvbuf, '#');
		if (p == NULL) {
			LogFatal("Not get pipe gap \"#\"");
		}
		sscanf(rcvbuf, "%d", *pfd);
		int len = p - rcvbuf + 1;
		// unuseful data
		error = recv(ppp->fd[1], rcvbuf, len, 0);
		if (error == -1) {
			perror("Recv");
			LogFatal("Recv fd from Pipe failed with 0 with PEEK flg");
		}
	}

	error = pthread_mutx_unlock(&ppp->mutx);
	if (error == -1) {
		perror("Unlock");
		LogFatal("Try to unlock mutex when recv fd failed");
	}
}

void pipe_close_read(Pipe *ppp) {
	close(ppp->fd[0]);
}

void pipe_close_write(Pipe *ppp) {
	close(ppp->fd[1]);
}
