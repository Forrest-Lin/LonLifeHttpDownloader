DIR = $(shell pwd)
CC = gcc
SRC = map.c rbTree.c 
HEADER = $(DIR)/*.h
OBJ := $(patsubst %.c,%.o, $(SRC))
NAME = map
TESTC = test.c
LIB_OBJ = libmap.a
TEST = test

all:  $(LIB_OBJ) $(TEST)

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC)

$(LIB_OBJ): $(OBJ) $(HEADER)
	ar rcs $(LIB_OBJ) $(OBJ)

$(TEST): $(TESTC)
	gcc -o $(TEST) $(TESTC) -L. -l$(NAME)

clean:
	rm -rf $(OBJ)
	rm -rf $(LIB_OBJ)
	rm -rf $(TEST)

rm:
	rm -rf $(OBJ) $(TEST)
