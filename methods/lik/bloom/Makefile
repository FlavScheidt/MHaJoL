CC=gcc
CFLAGS=-g -Wall -fopenmp -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid
DEPS = ../../../hash/hash.h ../join.h ../bloomFilter.h

csrc = $(wildcard ../../../hash/*.c) \
		$(wildcard ../*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

bloom: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../../hash/hash
	rm -f ../*.o ../BloomFilterx86
	rm -f *.o bloom