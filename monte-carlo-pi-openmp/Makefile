CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra -g -fopenmp
LIBS = -lm


all: pi

pi: *.c
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<
