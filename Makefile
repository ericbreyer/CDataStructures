HEADERS = ../*.h  ../*/*.h ../*/*/*.h
OBJECTS = ./test/test.o ./rbtree/rbtree.o ./rbtree/rbnode/rbnode.o ./llist/llist.o ./linked_hash_table/linked_hash_table.o ./linked_hash_table/chaining_list/chaining_list.o

CC=gcc 
CFLAGS= -Wall -Wextra -g -Wno-void-pointer-to-int-cast -Wno-int-conversion # -Wno-unused-command-line-argument

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