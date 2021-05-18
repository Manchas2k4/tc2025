/**
* To compile: gcc pthrads1.c -lpthread
**/

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define SIZE 		10
#define MAXNUM 	10000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t space_available = PTHREAD_COND_INITIALIZER;
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER;

int b[SIZE];
int size = 0;
int front = 0, rear = 0;

void add_buffer(int i) {
	b[rear] = i;
	rear = (rear + 1) % SIZE;
	size++;
}

int get_buffer(){
	int v;
	v = b[front];
	front= (front + 1) % SIZE;
	size--;
	return v ;
}

void* producer(void *arg) {
	int i;

	printf("producter starting...\n");
	i = 0;
	while (1) {
		pthread_mutex_lock(&mutex);
		if (size == SIZE) {
			pthread_cond_wait(&space_available, &mutex);
		}
		printf("producer adding %i...\n", i);
		add_buffer(i);
		pthread_cond_signal(&data_available);
		pthread_mutex_unlock(&mutex);
		i = (i + 1) % MAXNUM;
		sleep(1);
	}
	pthread_exit(NULL);
}

void* consumer(void *arg) {
	int i, v;
	printf("consumer starting...\n");
	for (i = 0; i < 100; i++) {
		pthread_mutex_lock(&mutex);
		if (size == 0) {
			pthread_cond_wait(&data_available, &mutex);
		}
		v = get_buffer();
		printf("consumer getting %i...\n", v);
		pthread_cond_signal(&space_available);
		pthread_mutex_unlock(&mutex);
	}
	printf("consuming finishing...\n");
	pthread_exit(NULL);
}

int main(int argc, char* argv[])   {
	pthread_t producer_thread;
	pthread_t consumer_thread;

	pthread_create(&producer_thread, NULL, producer, NULL);
	sleep(10);
	pthread_create(&consumer_thread, NULL, consumer, NULL);
	pthread_join(consumer_thread, NULL);
	return 0;
}
