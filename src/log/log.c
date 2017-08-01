#include "log.h"
int LOGFILE = -1;

void SetLogFile(const char *filename) {
	char *full_name = (char *)lalloc(22, 1);
	strcat(full_name, "../../log/");
	strcat(full_name, filename);
	LOGFILE = open(full_name, O_WRONLY|O_APPEND, 0644);
	if (LOGFILE == -1) {
		LogWarning("Pointed log file not exist will create it");
		LOGFILE = open(full_name, O_WRONLY|O_APPEND|O_CREAT, 0644);
		if (LOGFILE == -1) {
			LogFatal("Cannot create new log file and exit");
			exit(-1);
		}
	}

	lfree(full_name);
	LogNotice("Start write log message into file");
}

void get_time(char *timer) {
	time_t timep;
	struct tm *p_tm = NULL;
	time(&timep);
	p_tm = localtime(&timep); 
	sprintf(timer, "[%d-%d-%d:%d:%d]", 1+p_tm->tm_mon, 
			p_tm->tm_mday, 8+p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
}

void LogNotice(const char *msg) {
	const char *prefix = "[NOTICE]:";
	char *timer = (char *)lalloc(22, 1);
	get_time(timer);
	if (LOGFILE != -1) {
		write(LOGFILE, prefix, strlen(prefix));
		write(LOGFILE, msg, strlen(msg));
		write(LOGFILE, timer, strlen(timer));
		write(LOGFILE, "\n", 1);
	}	

	// log terminal
	printf("\033[1;;32m%s\033[0m", prefix);
	printf("\033[1;;32m%s\033[0m", msg);
	printf("\033[1;;32m%s\033[0m\n", timer);
	lfree(timer);
}

void LogFatal(const char *msg) {
	const char *prefix = "[FATAL]:";
	char *timer = (char *)lalloc(22, 1);
	get_time(timer);
	if (LOGFILE != -1) {
		write(LOGFILE, prefix, strlen(prefix));
		write(LOGFILE, msg, strlen(msg));
		write(LOGFILE, timer, strlen(timer));
		write(LOGFILE, "\n", 1);
	}	

	// log terminal
	printf("\033[1;;31m%s\033[0m", prefix);
	printf("\033[1;;31m%s\033[0m", msg);
	printf("\033[1;;31m%s\033[0m\n", timer);
	close(LOGFILE);
	lfree(timer);
	exit(-1);
}

void LogWarning(const char *msg) {
	const char *prefix = "[WARN]:";
	char *timer = (char *)lalloc(22, 1);
	get_time(timer);
	if (LOGFILE != -1) {
		write(LOGFILE, prefix, strlen(prefix));
		write(LOGFILE, msg, strlen(msg));
		write(LOGFILE, timer, strlen(timer));
		write(LOGFILE, "\n", 1);
	}	

	// log terminal
	printf("\033[1;;33m%s\033[0m", prefix);
	printf("\033[1;;33m%s\033[0m", msg);
	printf("\033[1;;33m%s\033[0m\n", timer);
	lfree(timer);
}
