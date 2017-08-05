#ifndef _PIPE_H_
#define _PIPE_H_
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../log/log.h"

// thread pip
typedef struct Pipe{
	int fd[2];
	pthread_mutex_t mutx;
}Pipe;

// pipe data 5#10#
void init_pipe(Pipe *);

// send str
void pipe_send(Pipe *, int fd);
// read num:fd
void pipe_read(Pipe *, int *pfd);
void pipe_close_read(Pipe *);
void pipe_close_write(Pipe *);
#endif
