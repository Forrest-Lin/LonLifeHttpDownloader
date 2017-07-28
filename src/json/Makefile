SRC = light_json.c
HEADER = *.h
OBJ = light_json.o
TESTC = test_json.c
MPATH = ../map/
DEP = map

all : libjson.a test

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC)

libjson.a: $(OBJ)
	ar crs libjson.a $(OBJ)

test: $(TESTC)
	gcc -o test $(TESTC) -L. -ljson -L$(MPATH) -l$(DEP)

clean:
	rm -rf *.o
	rm -rf test
	rm -rf libjson.a

rm:
	rm -rf *.o
	rm -rf test
