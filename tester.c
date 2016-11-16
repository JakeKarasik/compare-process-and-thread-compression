#include <stdio.h>
#include <stdlib.h>

void compressR_LOLS(char * file, int parts);

int main(int argc, char ** argv){
	
	int choice = 0;
	printf("Greetings! What would you like to do with this file?\n");
	printf("1 -> Compress by processes\n");
	printf("2 -> Compress by threads\n");
	printf("0 -> Quit\n");
	scanf("%d", &choice);
	char * fileName = (char*)malloc(sizeof(char)*100);
	int numParts = 0;
	switch(choice){
		
		case 0 : break;
		case 1 : 
				printf("Please enter the filename of the file you want compressed--\n");
				scanf("%s", fileName);
				printf("How many parts do you want it compressed into?--\n"); 
				scanf("%d", &numParts);
				compressR_LOLS(fileName, numParts);
				break;
		case 2 : //compressT
				printf("Please enter the filename of the file you want compressed--\n");
				scanf("%s", fileName);
				printf("How many parts do you want it compressed into?--\n"); 
				scanf("%d", &numParts);
				//compressT_LOLS(fileName, numParts);
		        break;
				  
	}
	free(fileName);
	printf("Thank you for playing!\n");
	return 0;
		
		
}				
