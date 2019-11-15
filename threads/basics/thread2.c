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
	int i, limit;
	int *acum = (int*) malloc(sizeof(int));

	*acum = 0;
	limit = *( (int *) param );
	for (i = 1; i <= limit; i++) {
		(*acum) = (*acum) + i;
	}
	printf("PID = %i - THREAD %i - acum = %i\n", getpid(), (int) pthread_self(), (*acum));
	sleep( 1 );
	return ( (void*) acum);
}

int main(int arg, char* argv[]) {
	pthread_t pthread_id[10];
	int i, limit;
	int* ret;

	printf("PID = %i - creating threads\n", getpid());
	for (i = 0; i < 10; i++) {
	    limit = (i + 1) * 5;
		pthread_create(&pthread_id[i],
			            NULL, task, (void *) &limit);
	}
	for (i = 0; i < 10; i++) {
		pthread_join(pthread_id[i], (void *) ret);
	}
	printf("PID = %i - all threads are terminated\n", getpid());
	return 0;
}
