#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>
#include "compress.h"

/*

This is the multi-process version of compression.  The main setup
is that a parent process spawns off additional worker child 
processes and reaps them at the end.

*/
void compressR_LOLS(char * file, int parts) {

	//Checks if an empty or invalid string is given as input; gives an error.
	if (file == NULL) {
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	//Checks if someone tries to compress a file into a negative number of parts.
	if (parts < 1) {
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * orig; //File to be read from and compressed.
	orig = fopen(file, "r");

	//Checks to see if file exists
	if (orig == NULL) {
		fprintf(stderr,"Error: Unable to access file.\n");
		exit(EXIT_FAILURE);
	}

	fseek(orig, 0, SEEK_END);
	int characters_in_file = ftell(orig); //# of characters in file
	int segment_partition = characters_in_file / parts; //# of characters per part
	int remainder = characters_in_file % parts; //If > 0, adds to first part
	fclose(orig);

	//Checks if someone tries to compress a file into more parts than characters
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

	//Loops through spawning child worker processes via compressR_worker_LOLS to compress multiple
	//partitions.  If there is only 1, then only 1 process is spawned without a number after the name.
	for(j = 0; j < parts; j++){
		
		if (parts == 1) { //Single part
			part_number = -1;
		} else { //Multiple parts
			part_number = j;
		}

		if (j == 0){ //First part, add remainder to size
			start_pos = 0;
			end_pos = segment_partition + remainder - 1;
		} else { //Every part after first, set size w/o remainder
			start_pos = prev;
			end_pos = segment_partition + prev - 1;
		}
			
		prev = end_pos + 1;
		current = fork();
		
		//If the current process is not the parent, the it does the compression.
		//The start, end, partion number and uncompressed file name for each compressed file is passed to the compressR_worker_LOLS executable.
		if(current == 0){
			
			char part_num_buff[length_of_int(part_number)];
			char start_pos_buff[length_of_int(start_pos)];
			char end_pos_buff[length_of_int(end_pos)];
			
			sprintf(part_num_buff, "%d", part_number);
			sprintf(start_pos_buff, "%d", start_pos);
			sprintf(end_pos_buff, "%d", end_pos);

			char * args[] = {"./compressR_worker_LOLS",start_pos_buff,end_pos_buff,part_num_buff,file, NULL};
			execvp("./compressR_worker_LOLS", args);			
			
		} else if (current == -1) { //If fork fails... the world is ending
			fprintf(stderr,"Error: Unable to fork().\n");
			exit(EXIT_FAILURE);
		}
		
		child_processID[j] = current;
	}

	//Loops to wait for all created children worker processes.
	j = 0;
	while(j < parts){
		
		int status;
		waitpid(child_processID[j], &status , 0);
		j++;
		
	}
}