#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <stdbool.h> 
#include "../json/light_json.h"
#include "../memory/alloc.h"
#include "../log/log.h"
#include "algorithm.h"
#include <fcntl.h>

// need a json file in etc/server.json
void connect_servers();
///////////////////
//construct a json_data

// file_path is like /doc/index.html
// you should provide enough space for push_string
void build_push_string(const char *file_path, char *push_string);
void build_del_string(const char *file_path, char *del_string);
void build_get_string(const char *file_path, char *get_string);
///////////////////
void send_file(int clientfd, const char *filename);
#endif
