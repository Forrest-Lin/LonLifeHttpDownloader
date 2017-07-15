#include "../map/map.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

Map parse_request(const char *request);
const char *get_request_method(Map *pmap);
bool judge_file_exsit(const char *filename, char *res);
const char *get_dest_file(Map *pmap);
