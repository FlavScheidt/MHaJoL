CC=gcc
CFLAGS=-g -Wall
DEPS = load.h oat_hash.h hash_any.h join.h MurmurHash2.h oat_hash.h bloomFilter.h
OBJ = load.o oat_hash.o hash_any.o join.o MurmurHash2.o bloomFilter.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

BloomFilterx86: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o BloomFilterx86