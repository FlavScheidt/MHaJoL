CC=gcc
CFLAGS=-g -Wall
DEPS = ../../hash/hash.h ../../join.h

csrc = $(wildcard ../../hash/*.c) \
		$(wildcard ../../*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

nested: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../hash/hash
	rm -f ../../*.o ../../BloomFilterx86
	rm -f *.o nested