OBJ = alloc.o
NAME = alloc
LIB = liballoc.a
TEST = test
SRC = alloc.c


all: $(TEST) $(LIB)

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC)

$(LIB): $(OBJ) 
	ar crs $(LIB) $(OBJ)

$(TEST): $(LIB) test.c
	gcc -o $(TEST) test.c -L. -l$(NAME)

clean: 
	rm -rf $(LIB) $(OBJ) $(TEST)

rm:
	rm -rf $(OBJ) $(TEST)
