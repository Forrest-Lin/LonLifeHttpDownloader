all: version

version: server_version1.c
	gcc -o version server_version1.c parse_http.c send_file.c -g -L../map -lmap -L../memory -lalloc

run: version
	./version

clean:
	rm -rf version
