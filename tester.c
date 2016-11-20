#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "compress.h"
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv){
	
	/*
	int choice = 0;
	printf("Greetings! What would you like to do with this file?\n");
	printf("1 -> Compress by processes\n");
	printf("2 -> Compress by threads\n");
	printf("0 -> Quit\n");
	scanf("%d", &choice);*/
	char * fileName = "./information.txt";//(char*)malloc(100);
	int numParts;
	/*switch(choice){
		
		case 0 : break;
		case 1 : 
				//printf("Please enter the filename of the file you want compressed--\n");
				//scanf("%s", fileName);
				printf("How many parts do you want it compressed into?--\n"); 
				scanf("%d", &numParts);
				compressR_LOLS(fileName, numParts);
				break;
		case 2 : //compressT
				//printf("Please enter the filename of the file you want compressed--\n");
				//scanf("%s", fileName);
				printf("How many parts do you want it compressed into?--\n"); 
				scanf("%d", &numParts);
				compressT_LOLS(fileName, numParts);
		        break;
				  
	}
	*/
	numParts = 5;

	long time = 0;
	struct timeval start, finish, start2, finish2;
	gettimeofday(&start, NULL);
	compressR_LOLS(fileName, numParts);
	gettimeofday(&finish, NULL);

	time += (long)(finish.tv_sec - start.tv_sec)*1000000L;
	time += (long)(finish.tv_usec - start.tv_usec);
	printf("Process time = %ld microseconds\n",time);

	time = 0;
	gettimeofday(&start2, NULL);
	compressT_LOLS(fileName, numParts);
	gettimeofday(&finish2, NULL);
	time += (long)(finish2.tv_sec - start2.tv_sec)*1000000L;
	time += (long)(finish2.tv_usec - start2.tv_usec);
	printf("Thread time = %ld microseconds\n",time);
	//free(fileName);
	
	return 0;	
} 