CC=gcc
CFLAGS=-g -Wall
DEPS = ../hash/hash.h ../load.h 

csrc = $(wildcard ../hash/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

hashTest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../hash/hash
	rm -f *.o hashTest