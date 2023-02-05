HEADERS = ../*.h  ../*/*.h ../*/*/*.h
OBJECTS = ./test/test.o ./rbtree/rbtree.o ./rbtree/rbnode/rbnode.o ./llist/llist.o

CC=gcc 
CFLAGS= -Wall -Wextra -g

.PHONY: clean all default run debug bloom

default: test

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

test: $(OBJECTS)
	$(CC) $(OBJECTS) -o ./test/bin/$@.exe $(CFLAGS)

run: test
	./test/bin/test.exe

debug: test
	lldb ./test/bin/test.exe

clean:
	-rm -f $(OBJECTS)
	-rm -f ./test/bin/test.exe