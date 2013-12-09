CC=gcc
MCC=mpicc
CFLAGS=-O2
LDFLAGS=-lcrypto

.PHONY: clean

all: caseq mpi-caseq

caseq: caseq.c random.c md5tool.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

mpi-caseq: mpi-caseq.c random.c md5tool.c
	$(MCC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -rf *.o
	rm -rf caseq
	rm -rf mpi-caseq
