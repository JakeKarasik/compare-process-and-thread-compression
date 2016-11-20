#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

//Worker function called by pthread create
void * thread_worker(file_data * thread_data) {

	LOLS(thread_data->start_pos, thread_data->end_pos,thread_data->part_number,thread_data->file_name);

	free(thread_data->file_name);
	free(thread_data);
	return NULL;
}

/*

This is the multi-thread version of compression.  The main setup
is that a main thread creates new threads to do the compression and then
reaps them at the end.

*/
void compressT_LOLS(char * file_name, int parts) {
	
	//Checks if an empty or invalid string is given as input; gives an error.
	if (file_name == NULL) {
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	//Checks if someone tries to compress a file into a negative number of parts.
	if (parts < 1) {
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * file = fopen(file_name, "r"); //File to be read from and compressed.

	//Checks to see if file exists
	if (file == NULL) {
		fprintf(stderr,"Error: Unable to access file.\n");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	int characters_in_file = ftell(file); //# of characters in file
	fclose(file);

	//Checks if someone tries to compress a file into more parts than characters
	if (parts > characters_in_file) {
		fprintf(stderr,"Error: There are more parts than there are characters in the file.\n");
		exit(EXIT_FAILURE);
	}

	int chars_per_thread = characters_in_file / parts; //# of characters per part
	int remainder = characters_in_file % parts;//If > 0, adds to first part
	int prev = 0;
	pthread_t child_threadID[parts];
	int i;

	//Loops through creating new threads via thread_worker to compress multiple
	//partitions.  If there is only 1, then only 1 thread is created without a number after the name.
	for (i=0;i<parts;i++) {

		pthread_t thread_id;
		file_data * data = malloc(sizeof(file_data)); 
		data->file_name = malloc(strlen(file_name)+1);
		strcpy(data->file_name,file_name);

		if (parts == 1) { //Single part
			data->part_number = -1;
		} else { //Multi parts
			data->part_number = i;
		}

		if (i == 0) { //First part, add remainder to size
			data->start_pos = 0; 
			data->end_pos = chars_per_thread + remainder - 1;
		} else { //Every part after first, set size w/o remainder
			data->start_pos = prev;
			data->end_pos = chars_per_thread + prev - 1;
		}
		pthread_create(&thread_id, NULL, (void * (*)(void *))thread_worker, data); //Create new thread

		child_threadID[i] = thread_id; //Save threadid to join it later

		prev = data->end_pos + 1;
	}

	i = 0;
	//Loops to join all created threads
	while(i < parts) {
		pthread_join(child_threadID[i], NULL);
		i++;
	}
}