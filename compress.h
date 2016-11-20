#ifndef COMPRESS_H
#define COMPRESS_H

/*

This header contains the struct prototype for file_data, which is used
to pass specific information on the section of data to be compressed from
the input file compressed by multithreading.  The subsequent prototypes 
refer to the actual functions that operate on said information.

*/

typedef struct file_data_ {
	
	int start_pos;//This refers to the beginning location in the file to be read for a partition.
	int end_pos;//This refers to teh end of the partition in the file to be read.
	int part_number;//This is the index of the compressed file if there is more than one; it is -1 if there is only 1 made.
	char * file_name;//This is the name of the file that is to be compressed that is passed into fopen.
	
} file_data;

void compressT_LOLS(char *, int);//The multi-thread version of compression; more details in compressT_LOLS.c.
void compressR_LOLS(char *, int);//The multi-process version of compression; more details in compressR_LOLS.c.
void LOLS(int, int, int, char *);//The base algorithm for compression; more details in functions.c.
int length_of_int(int);//This determins the length of the indices of each compressed file; more details in functions.c.
void * thread_worker(file_data *);//This does the work for multi-threading/ more details in compressT_LOLS.c.

#endif