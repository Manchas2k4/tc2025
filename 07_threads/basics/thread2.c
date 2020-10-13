/**
* To compile: gcc pthread.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 

void* task(void* param) {
	int i, limit;
	int *acum = (int*) malloc(sizeof(int));

	*acum = 0;
	limit = *( (int *) param );
	printf("THREAD %li - limit = %i\n", (long int) pthread_self(), limit);
	for (i = 1; i <= limit; i++) {
		(*acum) = (*acum) + i;
	}
	printf("PID = %i - THREAD %li - acum = %i\n", getpid(), (long int) pthread_self(), (*acum));
	pthread_exit((void*) acum);
}

int main(int arg, char* argv[]) {
	pthread_t pthread_id[10];
	int i, limit;
	int* ret;

	printf("PID = %i - creating threads\n", getpid());
	for (i = 0; i < 10; i++) {
	    limit = (i + 1) * 10;
		pthread_create(&pthread_id[i],
			            NULL, task, (void *) &limit);
	}
	for (i = 0; i < 10; i++) {
		pthread_join(pthread_id[i], (void *) ret);
		printf("ret = %p\n", (ret));
        sleep(1);
	}
	printf("PID = %i - all threads are terminated\n", getpid());
	return 0;
}
