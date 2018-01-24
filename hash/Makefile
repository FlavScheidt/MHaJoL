CC=gcc
CFLAGS=-g -Wall 
DEPS = hash.h
OBJ = murmur2.o murmur3.o hash_any.o jenkins.o pearson.o fnv1a.o elfHash.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hash: $(OBJ)

.PHONY: clean

clean:
	rm -f *.o hash