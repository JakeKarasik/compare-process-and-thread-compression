#CC     =       cc
CC      =       gcc
FLAGS   =       -Wall -g

COMPILE =       $(CC) $(FLAGS)

all: compressT_LOLS compressR_LOLS

compressT_LOLS: compressT_LOLS.c
	$(COMPILE) compressT_LOLS.c -o compressT_LOLS

compressR_LOLS: compressR_worker_LOLS.c
	$(COMPILE) compressR_worker_LOLS.c -o compressR_LOLS 
	
clean:
	rm -rf *.o compressR_LOLS compressT_LOLS
