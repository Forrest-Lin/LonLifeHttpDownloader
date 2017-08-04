all: version

version: server.c process.c parse_http.c send_file.c semaphore.c
	gcc -o version server.c parse_http.c send_file.c process.c semaphore.c -g -L../map -lmap -L../memory -lalloc -L../log -llog -L../json -ljson

run: version
	./version

clean:
	rm -rf version
