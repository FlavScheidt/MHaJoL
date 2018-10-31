CC=gcc
CFLAGS=-g -Wall -fopenmp -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid  -O2 -ftree-vectorize -fopt-info-vec-missed
DEPS = ../../../hash/hash.h ../../../join.h ../../../cuckoo.h

csrc = $(wildcard ../../../hash/*.c) \
		$(wildcard ../../../*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

cuckoo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ../../../hash/*.o ../../../hash/hash
	rm -f ../../../*.o 
	rm -f *.o cuckoo