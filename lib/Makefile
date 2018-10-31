CC=gcc
CFLAGS=-g -Wall -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid
DEPS = functions.h hashEssentials.h load.h join.h hash/hash.h cuckoo.h cht.h

csrc = $(wildcard hash/*.c) \
       $(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

MHaJoL: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o hash/hash
	rm -f *.o MHaJoL