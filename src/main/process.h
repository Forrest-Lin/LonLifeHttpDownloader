#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "parse_http.h"
#include "send_file.h"
#include "../log/log.h"
#include "../memory/alloc.h"
#include "semaphore.h"
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

typedef struct msgbuf{
	long mtype;
	char mtext[512];
}msgbuf;
void multiprocess_start();
#endif
