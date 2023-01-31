HEADERS = ../rbtree/rbtree.h
OBJECTS = ./test/test.o ./rbtree/rbtree.o

CC=gcc 
CFLAGS= -Wall -Wextra

.PHONY: clean all default run

default: test

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

test: $(OBJECTS)
	$(CC) $(OBJECTS) -o ./test/bin/$@.exe $(CFLAGS)

run:
	./test/bin/test.exe

clean:
	-rm -f $(OBJECTS)
	-rm -f ./test/bin/test.exe