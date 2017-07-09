DIR = $(shell pwd)
CC = gcc
SRC := $(wildcard  $(DIR)/*.c)
HEADER = $(DIR)/*.h
OBJ := $(patsubst %.c,%.o, $(SRC))
LIB_OBJ = libmap.a
SO_OBJ = map.so

all:  $(SO_OBJ) $(LIB_OBJ)

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC)

$(SO_OBJ):$(OBJ) $(HEADER)
	$(CC) $(OBJ) -shared -fPIC -o $(SO_OBJ)

$(LIB_OBJ): $(OBJ) $(HEADER)
	ar rcs $(LIB_OBJ) $(OBJ)

clean:
	rm -rf $(OBJ)
	rm -rf $(LIB_OBJ)
	rm -rf $(SO_OBJ)
