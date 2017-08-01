#include "../map/map.h"
#include "../log/log.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

Map parse_request(const char *request);
const char *get_request_method(Map *pmap);
// res == NULL, if wont res the full path
// filename is like /doc/index.html
bool judge_file_exsit(const char *filename, char *res);
const char *get_dest_file(Map *pmap);
void clear_node(void *);
void show_node(void *);
