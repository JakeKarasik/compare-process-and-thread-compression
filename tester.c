#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "compress.h"
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv){
	
	int choice = 0;
	printf("Greetings! What would you like to do with this file?\n");
	printf("1 -> Compress by processes\n");
	printf("2 -> Compress by threads\n");
	printf("0 -> Quit\n");
	scanf("%d", &choice);
	char * fileName = "information.txt";//(char*)malloc(100);
	int numParts;
	switch(choice){
		
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
	/*
	numParts = 5;

	long time = 0;
	struct timeval start, finish;
	gettimeofday(&start, NULL);

		compressR_LOLS(fileName, numParts);

	gettimeofday(&finish, NULL);

	time += (long)(finish.tv_sec - start.tv_sec)*1000000L;
	time += (long)(finish.tv_usec - start.tv_usec);
	printf("Process time = %ld \n",time);

	int i=0;
	char * str = "information_txt_LOLS";
	for (;i<numParts;i++) {
		char buff[100];
		strcat(buff,str);
		sprintf(buff, "%d", i);
		if(access(buff, F_OK) != -1) {
	        remove(buff);
	        printf("removing file....%S",buff);
   	 	}
	}
	

	time = 0;
	gettimeofday(&start, NULL);

		compressT_LOLS(fileName, numParts);

	gettimeofday(&finish, NULL);
	printf("Thread time = %ld \n",time);*/
	//free(fileName);
	//printf("Thank you for playing!\n");
	return 0;
		
		
} 