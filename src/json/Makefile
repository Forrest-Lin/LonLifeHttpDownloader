SRC = light_json.c
HEADER = *.h
OBJ = light_json.o
TESTC = test_json.c
HEADERPATH = ../../include
MPATH = ../map/
DEP = map
DEPARG = -L../memory -lalloc

all : libjson.a test

$(OBJ): $(SRC)
	gcc -fPIC -c $(SRC)

libjson.a: $(OBJ)
	ar crs libjson.a $(OBJ)

test: $(TESTC) libjson.a
	gcc -o test $(TESTC) -L. -ljson -L$(MPATH) -l$(DEP) $(DEPARG)
	cp $(HEADER) $(HEADERPATH)

clean:
	rm -rf *.o
	rm -rf test
	rm -rf libjson.a

rm:
	rm -rf *.o
	rm -rf test
