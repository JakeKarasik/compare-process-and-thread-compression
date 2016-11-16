#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int length_of_int(int n) {
    if (n < 10) { 
    	return 1;
    } else if (n < 100) {
    	return 2;
    } else if (n < 1000) {
    	return 3;
    } else if (n < 10000) {
    	return 4;
    } else if (n < 100000) {
    	return 5;
    } else if (n < 1000000) {
    	return 6;
    } else if (n < 10000000) {
    	return 7;
    } else if (n < 100000000) {
    	return 8;
    } else if (n < 1000000000) {
    	return 9;
    } else {
    	return 10;
    }
}

void compressR_LOLS(char * file, int parts){
	
	//printf("Working\n");
	FILE * orig;
	orig = fopen(file, "r");
	fseek(orig, 0, SEEK_END);
	int segment_partition = ftell(orig) / parts;
	int remainder = ftell(orig) % parts;
	fseek(orig, 0, SEEK_SET);
	fclose(orig);
	
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
		//printf("Child process current = %d\n", current);
		
		
	}
	j = 0;
	while(j < parts){
		
		int status;
		waitpid(child_processID[j], &status , 0);
		//printf("Child %d status = %d\n", child_processID[j], status);
		j++;
		
	}
	return;
	
}

	
