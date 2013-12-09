MCC=mpicc
CFLAGS=-O2

.PHONY: clean

all : hello-world scatter 


hello-world : hello-world.c
	$(MCC) $(CFLAGS) -o $@ $^

scatter : scatter.c
	$(MCC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o
	rm -rf hello-world
	rm -rf scatter

