CC=gcc
RANDOMFLAGS=-mcmodel=large #no idea why I need those, but it works, so keep it
DEBUGFLAGS=#-g -pg -Wall -Wextra -pedantic #-lefence -L/home/fscristo/usr/lib
ARCHFLAGS=-march=skylake-avx512
PROFFLAGS= -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid

CFLAGS= -lpthread $(DEBUGFLAGS) $(RANDOMFLAGS) $(PROFFLAGS) $(ARCHFLAGS) -O2

DEPS = ../../lib/hash/hash.h ../lib/../join.h ../../cuckoo.h

csrc = $(wildcard ../../lib/hash/*.c) \
		$(wildcard ../../lib/*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

scalar: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../lib/hash/hash
	rm -f *.o scalar
	rm -f ../../lib/*.o