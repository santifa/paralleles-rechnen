CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra -O2 -g -fopenmp
LIBS = -lm -lrt


all: pi

pi: *.c
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<

clean:
	rm -f pi
