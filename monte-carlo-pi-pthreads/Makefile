CC = gcc
CFLAGS = -pthread -Wall -O2
LIBS = -lrt

all: pi


pi : *.c
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<


clean:
	rm -f pi
