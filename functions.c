#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//Returns number of digits in int 'n'
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

/*
LOLS compression algorithm.  It compresses a file given a start position, end position, part number
and filename.  The part_number is the index of the compressed file being made.
The algorithm goes through the indicated range and copies to the compressed file only 
alphabetical characters.  All other symbols are ignored.  WARNING!  If a compressed 
file of the original file already exists with the same part number,it will be removed.  
*/
void LOLS(int start, int end, int part_number, char * file_name) {

    int length_of_part_number = part_number < 0 ? 0 : length_of_int(part_number);
    char * compress_name = malloc(strlen(file_name) + 1 + 5 + length_of_part_number); //filename + null term + "_LOLS" + part length
    memset(compress_name, 0, strlen(file_name) + 1 + 5 + length_of_part_number);
    //Changes last dot in filename to _ for compress_name
    int i = 0;
    char * extension_dot = strrchr(file_name,'.');
    int extension_dot_location;
    if (extension_dot != NULL) {
        extension_dot_location = extension_dot-file_name;
    } else {
        extension_dot_location = -1;
    }
    
    while(file_name[i] != '\0'){
        
        if (extension_dot_location == i) {
            compress_name[i] = '_';
        } else {
            compress_name[i] = file_name[i];
        }

        i++;
    }
    compress_name[i] = '\0';
    
    //Appends '_LOLS' to compress_name
    strcat(compress_name, "_LOLS");
    //If more than 1 part, adds part number
    if (part_number >= 0) {
        char part_as_string[length_of_part_number+1];
        sprintf(part_as_string, "%d", part_number);
        strcat(compress_name, part_as_string);
    }

    FILE * orig, * compress; //Orig is the file to compress and compress is the current compressed file being made.
    orig = fopen(file_name, "r");

    //If compressed file with same name exists, delete in preparation for new file.
    if(access(compress_name, F_OK) != -1) {
        //printf("Notice: Compressed file with this name exists; deleting old file.\n");
        remove(compress_name);
    }

    compress = fopen(compress_name, "w+");
    free(compress_name);

    char curr_char; //Current character in file
    char compare_char; //Char to compare current character to
    int counter = start;
    int num_of_chars = 1; //Tracks number of matching chars
    fseek(orig, start, SEEK_SET); //Move to the correct starting location in file
    compare_char = fgetc(orig);

    //If file begins with non-alphabetic character, loop until end of section or alphabetic char is found
    while (compare_char != EOF && counter <= end && !isalpha(compare_char)) {
        if (counter == end) {
            counter++;
            break;
        }
        compare_char = fgetc(orig);
        counter++;
    }

    //Loops through section, saving compressed chars into new compressed file
    for (i = 0; counter <= end && compare_char != EOF; i++) {
        curr_char = fgetc(orig);

        if(!isalpha(curr_char)){
            //Ignores non-alphabetic characters
        } else if(curr_char == compare_char) {
            //Keeps track of number of same chars
            num_of_chars++;

        } else {
            //LOL compression algorithm
            switch(num_of_chars) {
                case 1: 
                    fputc(compare_char, compress);
                    break;
                case 2: 
                    fputc(compare_char, compress);
                    fputc(compare_char, compress);
                    break;
                default:
                    fprintf(compress, "%d", num_of_chars);
                    fputc(compare_char, compress);
                    break; 
            }

            compare_char = curr_char;
            num_of_chars = 1;
            
        }
        counter++;
    }
    if (isalpha(curr_char)) {
        //Decrements so is not to overcount after the initial loop finishes.
        num_of_chars--;
    }
    //If loop ended but still 1 char to check, this ensures it is compressed
    if (isalpha(compare_char) && num_of_chars > 0) {
        switch(num_of_chars) {
            case 1: 
                fputc(compare_char, compress);
                break;
            case 2: 
                fputc(compare_char, compress);
                fputc(compare_char, compress);
                break;
            default:
                fprintf(compress, "%d", num_of_chars);
                fputc(compare_char, compress);
                break; 
        }
    }
    
    fclose(orig);
    fclose(compress);

}