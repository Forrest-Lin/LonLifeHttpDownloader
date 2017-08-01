DIR = $(shell pwd)
CC = gcc
SRC = scheduler.c algorithm.c
HEADER = $(DIR)/*.h
OBJ := $(patsubst %.c,%.o, $(SRC))
NAME = scheduler
TESTC = test.c
LIB_OBJ = libscheduler.a
TEST = test
DEPARG = -L../json -ljson -L../map -lmap -L../memory -lalloc -L../log -llog

all:  $(LIB_OBJ) $(TEST)

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC) -g

$(LIB_OBJ): $(OBJ) $(HEADER)
	ar rcs $(LIB_OBJ) $(OBJ)

$(TEST): $(TESTC) $(LIB_OBJ)
	gcc -o $(TEST) $(TESTC) ../main/parse_http.c -L. -l$(NAME) $(DEPARG) -g

clean:
	rm -rf $(OBJ)
	rm -rf $(LIB_OBJ)
	rm -rf $(TEST)

rm:
	rm -rf $(OBJ) $(TEST)
