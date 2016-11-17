#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    	return 19; //worst case 19 digits
    }
}

void LOLS(int start, int end, int part_number, char * file_name) {

    //printf("Part %d start=%d end=%d\n",part_number, start, end);

    int length_of_part_number = part_number < 0 ? 0 : length_of_int(part_number);
    char * compress_name = malloc(strlen(file_name) + 1 + 5 + length_of_part_number); //filename + null term + "_LOLS" + part length
    
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
        //printf("i=%d, currchar = %c,compare_char = %c, part_num=%d\n",i, curr_char, compare_char,part_number);
        if(curr_char == compare_char) {

            num_of_chars++;

        } else {
            switch(num_of_chars) {
                case 1: 
                    fputc(compare_char, compress);
                    //printf("saving %c in part %d\n",compare_char, part_number);
                    break;
                case 2: 
                    fputc(compare_char, compress);
                    fputc(compare_char, compress);
                    //printf("saving %c%c in part %d\n",compare_char,compare_char, part_number);
                    break;
                default:
                    fprintf(compress, "%d", num_of_chars);
                    fputc(compare_char, compress);
                    //printf("saving %d%c in part %d\n",num_of_chars,compare_char, part_number);
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
                //printf("saving %c in part %d\n",compare_char, part_number);
                break;
            case 2: 
                fputc(compare_char, compress);
                fputc(compare_char, compress);
                //printf("saving %c%c in part %d\n",compare_char,compare_char, part_number);
                break;
            default:
                fprintf(compress, "%d", num_of_chars);
                fputc(compare_char, compress);
                //printf("saving %d%c in part %d\n",num_of_chars,compare_char, part_number);
                break; 
        }
    }
    
    fclose(orig);
    fclose(compress);
    free(compress_name);
}