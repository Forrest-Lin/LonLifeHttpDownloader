#include "log.h"

int main() {
	SetLogFile("main.lg");
	LogNotice("Very good");
	return 0;
}
