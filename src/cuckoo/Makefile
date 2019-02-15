CC=gcc
CFLAGS=-g -Wall -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid -lpthread -mavx2 -march=skylake
DEPS = ../../lib/hash/hash.h ../lib/../join.h ../../lib/cuckoo.h

csrc = $(wildcard ../../lib/hash/*.c) \
		$(wildcard ../../lib/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

cuckoo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../lib/hash/hash
	rm -f *.o cuckoo
	rm -f ../../lib/*.o