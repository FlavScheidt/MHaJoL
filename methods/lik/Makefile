CC=gcc
CFLAGS=-g -Wall -fopenmp -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid
DEPS = functions.h hashEssentials.h load.h hashTable.h join.h bloomFilter.h ../../hash/hash.h cuckoo.h

csrc = $(wildcard ../../hash/*.c) \
       $(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

BloomFilterx86: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ../../hash/*.o ../../hash/hash
	rm -f *.o BloomFilterx86