DIR = $(shell pwd)
CC = gcc
SRC = scheduler.c algorithm.c thread.c pipe.c md5.c
HEADER = $(DIR)/*.h
OBJ := $(patsubst %.c,%.o, $(SRC))
NAME = scheduler
TESTC = main.c
LIB_OBJ = libscheduler.a
TEST = scheduler
DEPLIBS = ../json/libjson.a ../map/libmap.a ../memory/liballoc.a ../log/liblog.a
DEPARG = -L../json -ljson -L../map -lmap -L../memory -lalloc -L../log -llog -lpthread

all:  $(LIB_OBJ) $(TEST)

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC) -g

$(LIB_OBJ): $(OBJ) $(HEADER)
	ar rcs $(LIB_OBJ) $(OBJ)

$(TEST): $(TESTC) $(LIB_OBJ) $(DEPLIBS)
	gcc -o $(TEST) $(TESTC) ../main/parse_http.c -L. -l$(NAME) $(DEPARG) -g

clean:
	rm -rf $(OBJ)
