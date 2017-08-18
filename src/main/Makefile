SRC = server.c process.c parse_http.c send_file.c semaphore.c
CC = gcc
OBJ := $(patsubst %.c,%.o, $(SRC))
DEPLIBS = ../map/libmap.a ../memory/liballoc.a ../log/liblog.a ../json/libjson.a
DEPARG = -L../map -lmap -L../memory -lalloc -L../log -llog -L../json -ljson -lefence

all: server

$(OBJ): $(SRC)
	gcc -c $(SRC)

server: $(OBJ) $(DEPLIBS)
	gcc -o server $(OBJ) $(DEPARG)

clean: 
	rm -rf $(OBJ)
