#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * thread_worker(void * arg) {
	printf("Hello world, %s\n", (char *)arg);
	return arg;
}

void compressT_LOLS(char * file, int parts) {
	
	if (file == NULL) {
		printf("Error: Invalid filename.\n");
		return;
	}

	if (parts < 1) {
		printf("Error: Invalid number of parts.\n");
		return;
	}

	pthread_t thread_id;

	pthread_create(&thread_id, NULL, thread_worker, "test");

	pthread_join(thread_id, NULL);

	//printf("%s %d\n",file, parts);

}

int main(int argc, char ** argv){

	int num_of_parts = argv[1] == NULL ? 1 : atoi(argv[1]);
	compressT_LOLS("information.txt", num_of_parts);
	
	return 0;
}