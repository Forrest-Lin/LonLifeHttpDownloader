#ifndef _SEND_FILE_H_
#define _SEND_FILE_H_
#include "parse_http.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../json/light_json.h"

void write_to_client(int fd, const char *file_name);
void err_to_deal(int fd, const char *file_name);
void add_header(char *response, Map *pmap);
int get_file_size(const char *file_name);
// here I support [200 OK] [400 Bad Request] [404 Not Found] [500 Internal Server Error]
void get_header_msg(Map *pmap, int status, const char *mean, int length);
// here I support [200 OK] [400 Bad Request] [404 Not Found] [500 Internal Server Error]
Map init_status_map();
const char *get_status_mean(Map *, int);
void clear_status_map(Map *pmap);

//                 flg yes, no   , full file name
void compound_json(bool flg, const char *file_name, const char *response_header, const char *clien_sign, char *json_res);
#endif
