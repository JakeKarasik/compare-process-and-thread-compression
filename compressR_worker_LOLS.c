#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv){

	char * FILE_NAME = argv[1];
	char * compress_name = malloc((strlen(FILE_NAME) + 9)); //change size later
	int i=0;

	while(FILE_NAME[i] != '\0'){
		
		if(FILE_NAME[i] == '.') {
			compress_name[i] = '_';
		} else {
			compress_name[i] = FILE_NAME[i];
		}
		
		i++;
		
	}
	strcat(compress_name, "_LOLS");
	FILE * orig, * compress;
	orig = fopen(FILE_NAME, "r");
	compress = fopen(compress_name, "w");
	char currVal;
	char mainVal;
	char numOfValueString[6];
	int numOfVal = 1;
	mainVal = fgetc(orig);
	for (i = 0; mainVal != EOF; i++) {
		
		currVal = fgetc(orig);
		if(currVal == mainVal) {
			
			numOfVal++;
			
		} else {
			
			switch(numOfVal) {
				
				case 1: 
					fputc(mainVal, compress);
					break;
				case 2: 
					fputc(mainVal, compress);
					fputc(mainVal, compress);
					break;
				default: 
					sprintf(numOfValueString,"%d",numOfVal);
					fputs(numOfValueString, compress);
					fputc(mainVal, compress);
					break; 
			}
			mainVal = currVal;
			numOfVal = 1;
			
		}
	}
	fclose(orig);
	fclose(compress);
	free(compress_name);
	return 0;

}