CC=gcc
CFLAGS=-g -Wall -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid -lpthread -ftree-vectorize
DEPS = functions.h hashEssentials.h load.h join.h hash/hash.h cuckoo.h cht.h

csrc = $(wildcard hash/*.c) \
       $(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

MHaJoL: $(OBJ)
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o hash/hash
	rm -f *.o MHaJoL