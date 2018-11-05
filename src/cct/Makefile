CC=gcc
CFLAGS=-g -Wall -I../../lib -I../../lib/hash -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid -lpthread -ftree-vectorize #-fopt-info-vec-missed
DEPS = hash.h join.h cct.h

csrc = $(wildcard ../../lib/hash/*.c) \
		$(wildcard ../../lib/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

cct: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../lib/hash/hash
	rm -f *.o cct
	rm -f ../../lib/*.o