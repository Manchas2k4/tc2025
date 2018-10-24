/**
* To compile: gcc pthrads1.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define LIMIT 10

void* task(void* param) {
	int i;
	
	for (i = 1; i <= LIMIT; i++) {
		printf("PID = %i - THREAD %i - i = %i\n", getpid(), (int) pthread_self(), i);
	}
	sleep( 1 );
	return ( (void*) 0);
}

int main(int arg, char* argv[]) {
	pthread_t pthread_id[10];
	int i;
	void* ret;
	
	printf("PID = %i - creating threads\n", getpid());
	for (i = 0; i < 10; i++) {
		pthread_create(&pthread_id[i], 
			            NULL, task, (void *) 0);
	}
	for (i = 0; i < 10; i++) {
		pthread_join(pthread_id[i], &ret);
	}
	printf("PID = %i - all threads are terminated\n", getpid());
	return 0;
}
