#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct file_data_ {
	int start_pos;
	int end_pos;
	int part_number;
	char * file_name;
} file_data;

void compressT_LOLS(char *, int);
void compressR_LOLS(char *, int);
void LOLS(int, int, int, char *);
int length_of_int(int);
void * thread_worker(file_data *);

#endif