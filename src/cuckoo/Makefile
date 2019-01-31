CC=gcc
CFLAGS=-g -Wall -fopenmp -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid  -O2 -ftree-vectorize -fopt-info-vec-missed
DEPS = ../../lib/hash/hash.h ../../lib/join.h ../../lib/cuckoo.h

csrc = $(wildcard ../../lib/hash/*.c) \
		$(wildcard ../../lib/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

cuckoo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ../../lib/hash/*.o ../../lib/hash/hash
	rm -f ../../lib/*.o 
	rm -f *.o cuckoo