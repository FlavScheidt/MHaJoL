CC=gcc
CFLAGS=-g -Wall -DLIKWID_PERFMON -L$LIKWID_LIB -I$LIKWID_INCLUDE -llikwid -lpthread -ftree-vectorize #-fopt-info-vec-missed
DEPS = ../../../hash/hash.h ../../../join.h ../../../cct.h

csrc = $(wildcard ../../../hash/*.c) \
		$(wildcard ../../../*.c) \
       	$(wildcard ./*.c) \

OBJ = $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

cct: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f hash/*.o ../../../hash/hash
	rm -f *.o cct
	rm -f ../../../*.o