#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "compress.h"

void compressR_LOLS(char * file, int parts) {

	if (file == NULL) {
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	if (parts < 1) {
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * orig;
	orig = fopen(file, "r");

	if (orig == NULL) {
		fprintf(stderr,"Error: Invalid file.\n");
		exit(EXIT_FAILURE);
	}

	fseek(orig, 0, SEEK_END);
	int characters_in_file = ftell(orig);
	int segment_partition = characters_in_file / parts;
	int remainder = characters_in_file % parts;
	fclose(orig);

	if (parts > characters_in_file) {
		fprintf(stderr,"Error: There are more parts than there are characters in the file.\n");
		exit(EXIT_FAILURE);
	}

	int j;
	int part_number;
	int prev = 0;
	int start_pos, end_pos;
	pid_t child_processID[parts];
	pid_t current;
	for(j = 0; j < parts; j++){
		
		if (parts == 1) {
				part_number = -1;
			} else {
				part_number = j;
			}

			if (j == 0){
				start_pos = 0;
				end_pos = segment_partition + remainder - 1;
			} else {
				start_pos = prev;
				end_pos = segment_partition + prev - 1;
			}
			
		prev = end_pos + 1;
		current = fork();
		
		if(current == 0){
			
			char part_num_buff[length_of_int(part_number)];
			char start_pos_buff[length_of_int(start_pos)];
			char end_pos_buff[length_of_int(end_pos)];
			
			sprintf(part_num_buff, "%d", part_number);
			sprintf(start_pos_buff, "%d", start_pos);
			sprintf(end_pos_buff, "%d", end_pos);

			char * args[] = {"./Worker",start_pos_buff,end_pos_buff,part_num_buff,file, NULL};
			execvp("./Worker", args);			
			
		}
		
		child_processID[j] = current;
	}

	j = 0;
	while(j < parts){
		
		int status;
		waitpid(child_processID[j], &status , 0);
		j++;
		
	}
}