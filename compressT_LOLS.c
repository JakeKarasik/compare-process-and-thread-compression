#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/*
	To do:
	-Fix: not splitting properly, (possibly because of line 79)

*/

typedef struct file_data_ {
	int start_pos;
	int end_pos;
	int part_number;
	char * file_name;
} file_data;

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

void LOLS(int start, int end, int part_number, char * file_name) {

	printf("Part %d start=%d end=%d\n",part_number, start, end);

	int length_of_part_number = part_number < 0 ? 0 : length_of_int(part_number);
	char * compress_name = malloc(strlen(file_name) + 1 + 5 + length_of_part_number); //filename + null term + part length
	
	int i = 0;
	while(file_name[i] != '\0'){
		
		if (file_name[i] == '.') {
			compress_name[i] = '_';
		} else {
			compress_name[i] = file_name[i];
		}

		i++;
	}

	strcat(compress_name, "_LOLS");
	if (part_number >= 0) {
		char part_as_string[length_of_part_number];
		sprintf(part_as_string, "%d", part_number);
		strcat(compress_name, part_as_string);
	}
	
	FILE * orig, * compress;
	orig = fopen(file_name, "r");
	compress = fopen(compress_name, "w");

	char curr_char;
	char compare_char;
	int counter = start;
	int num_of_chars = 1;
	fseek(orig, start, SEEK_SET);
	compare_char = fgetc(orig);

	for (i = 0; counter <= end && compare_char != EOF; i++) {
		curr_char = fgetc(orig);
		printf("i=%d, currchar = %c,compare_char = %c, part_num=%d\n",i, curr_char, compare_char,part_number);
		if(curr_char == compare_char) {

			num_of_chars++;

		} else {
			switch(num_of_chars) {
				case 1: 
					fputc(compare_char, compress);
					printf("saving %c in part %d\n",compare_char, part_number);
					break;
				case 2: 
					fputc(compare_char, compress);
					fputc(compare_char, compress);
					printf("saving %c%c in part %d\n",compare_char,compare_char, part_number);
					break;
				default:
					fprintf(compress, "%d", num_of_chars);
					fputc(compare_char, compress);
					printf("saving %d%c in part %d\n",num_of_chars,compare_char, part_number);
					break; 
			}

			compare_char = curr_char;
			num_of_chars = 1;
			
		}
		counter++;
	}
	num_of_chars--;
	if (curr_char != EOF && num_of_chars > 0) {
		switch(num_of_chars) {
			case 1: 
				fputc(compare_char, compress);
				printf("saving %c in part %d\n",compare_char, part_number);
				break;
			case 2: 
				fputc(compare_char, compress);
				fputc(compare_char, compress);
				printf("saving %c%c in part %d\n",compare_char,compare_char, part_number);
				break;
			default:
				fprintf(compress, "%d", num_of_chars);
				fputc(compare_char, compress);
				printf("saving %d%c in part %d\n",num_of_chars,compare_char, part_number);
				break; 
		}
	}
	
	fclose(orig);
	fclose(compress);
	free(compress_name);
}

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

int main(int argc, char ** argv){

	int num_of_parts = argv[1] == NULL ? 1 : atoi(argv[1]);
	compressT_LOLS("information.txt", num_of_parts);
	
	return 0;
}