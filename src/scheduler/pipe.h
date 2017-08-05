#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "../log/log.h"

// thread pip
typedef struct Pipe{
	int fd[2];
	pthread_mutex mutx;
}Pipe;

// pipe data 5#10#
void init_pipe(Pipe *);

// send str
void pipe_send(Pipe *, int fd);
// read num:fd
void pipe_read(Pipe *, int *pfd);
void pipe_close_read(Pipe *);
void pipe_close_write(Pipe *);
