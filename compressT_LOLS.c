#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

void * thread_worker(file_data * thread_data) {

	LOLS(thread_data->start_pos, thread_data->end_pos,thread_data->part_number,thread_data->file_name);

	free(thread_data->file_name);
	free(thread_data);
	return NULL;
}

void compressT_LOLS(char * file_name, int parts) {
	
	if (file_name == NULL) {
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	if (parts < 1) {
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * file = fopen(file_name, "r");

	if (file == NULL) {
		fprintf(stderr,"Error: Invalid file.\n");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	int characters_in_file = ftell(file);
	fclose(file);

	if (parts > characters_in_file) {
		fprintf(stderr,"Error: There are more parts than there are characters in the file.\n");
		exit(EXIT_FAILURE);
	}

	int chars_per_thread = characters_in_file / parts;
	int remainder = characters_in_file % parts;
	int prev = 0;

	int i;
	for (i=0;i<parts;i++) {

		pthread_t thread_id;
		file_data * data = malloc(sizeof(file_data)); 
		data->file_name = malloc(strlen(file_name)+1);
		strcpy(data->file_name,file_name);

		if (parts == 1) {
			data->part_number = -1;
		} else {
			data->part_number = i;
		}

		if (i == 0){
			data->start_pos = 0;
			data->end_pos = chars_per_thread + remainder - 1;
		} else {
			data->start_pos = prev;
			data->end_pos = chars_per_thread + prev - 1;
		}
		pthread_create(&thread_id, NULL, (void * (*)(void *))thread_worker, data);

		pthread_detach(thread_id);

		prev = data->end_pos + 1;
	}
	pthread_exit(NULL);
}