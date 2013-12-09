all : hello-world scatter 

hello-world : hello-world.c
	mpicc -O2 -o hello-world hello-world.c

scatter : scatter.c
	mpicc -O2 -o scatter scatter.c

