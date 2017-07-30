DIR = $(shell pwd)
CC = clang 
SRC = log.c
HEADER = $(DIR)/*.h
OBJ := $(patsubst %.c,%.o, $(SRC))
NAME = log
TESTC = test.c
LIB_OBJ = liblog.a
TEST = test
DEPARG = -L../memory -lalloc

all:  $(LIB_OBJ) $(TEST)

$(OBJ): $(SRC)
	$(CC) -fPIC -c $(SRC)

$(LIB_OBJ): $(OBJ) $(HEADER)
	ar rcs $(LIB_OBJ) $(OBJ)

$(TEST): $(TESTC)
	$(CC) -o $(TEST) $(TESTC) -L. -l$(NAME) $(DEPARG)

clean:
	rm -rf $(OBJ)
	rm -rf $(LIB_OBJ)
	rm -rf $(TEST)

rm:
	rm -rf $(OBJ) $(TEST)
