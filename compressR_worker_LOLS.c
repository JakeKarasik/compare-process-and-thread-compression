#include <stdlib.h>
#include "compress.h"


/*

For the sake of modularity, we made this a separate exec file.  This is called
by compressR_LOLS.c to divide up the partitioning of a file to be compressed if
it split.  Otherwise, it just makes a new single compressed file.

*/
int main(int argc, char ** argv){
	
	LOLS(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),argv[4]);
	return 0;

}