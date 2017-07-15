
src := $(wildcard ./*.c)
obj := $(patsubst %.c,%.o, $(src))

all	:	test

test	:	$(obj) map.o rbTree.o
	gcc -o test $(obj) map.o rbTree.o

$(obj)	:	$(src)
	gcc -c $(src)
map.o	:	../map/map.c 
	gcc -c ../map/map.c
rbTree.o	:	../map/rbTree.c
	gcc -c ../map/rbTree.c
clean	:
	rm -rf *.o


