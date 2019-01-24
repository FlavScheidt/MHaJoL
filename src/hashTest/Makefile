CC=gcc
CFLAGS=-g -Wall -mavx2 -march=skylake
DEPS = ../../lib/hash/hash.h ../../lib/load.h 

csrc = $(wildcard ../../lib/hash/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

hashTest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../lib/hash/hash
	rm -f *.o hashTest