CC=gcc
CFLAGS=-g -Wall -I../../lib -I../../lib/hash -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid -lpthread #-mavx -ffast-math -ftree-vectorize -fopt-info-vec-optimized -fopenmp
DEPS = ../../lib/hash/hash.h ../lib/../join.h ../../lib/cpct.h

csrc = $(wildcard ../../lib/hash/*.c) \
		$(wildcard ../../lib/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

cpct: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../lib/hash/hash
	rm -f *.o cpct
	rm -f ../../lib/*.o