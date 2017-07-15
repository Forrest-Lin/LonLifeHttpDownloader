#ifndef _SEND_FILE_H_
#define _SEND_FILE_H_
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void write_to_client(int fd, const char *file_name);
void err_to_deal(int fd, const char *file_name);
void get_header_msg(Map *pmap, int status, const char *mean, int length);
#endif
