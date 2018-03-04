CC=gcc
CFLAGS=-g -Wall
DEPS = ../../hash/hash.h ../../join.h ../../bloomFilter.h ../../hashTable.h

csrc = $(wildcard ../../hash/*.c) \
		$(wildcard ../../*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

bloomChained: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../hash/hash
	rm -f ../../*.o ../../BloomFilterx86
	rm -f *.o bloomChained