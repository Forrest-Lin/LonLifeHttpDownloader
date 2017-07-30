#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include "../memory/alloc.h"

void LogFatal(const char *);
void LogNotice(const char *);
void LogWarning(const char *);
void SetLogFile(const char *);
#endif
