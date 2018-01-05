/**
* To compile: gcc producer-consumer.c -lpthread
**/

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 10

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
	front= (front+1) % SIZE;
	size--;
	return v ;
}
 
void* producer(void *arg) {
	int i = 0;
	
	printf("producter starting...\n");
	while (1) {
  		pthread_mutex_lock(&mutex);
  		if (size == SIZE) {
     		pthread_cond_wait(&space_available, &mutex);
  		}
  		printf("producer adding %i...\n", i);
		add_buffer(i);
		pthread_cond_signal(&data_available);
		pthread_mutex_unlock(&mutex);
		i = i + 1;
	}
	pthread_exit(NULL);
}

void* consumer(void *arg) {
	int i,v;
	printf("consumer starting...\n");
	for (i=0;i<20;i++) {
   		pthread_mutex_lock(&mutex);
   		if (size == 0) {
       		pthread_cond_wait(&data_available, &mutex); 
   		}
		v = get_buffer();
		printf("consumer %i getting %i...\n", ((int) pthread_self()), v);
		pthread_cond_signal(&space_available);
		pthread_mutex_unlock(&mutex);
	}
	printf("consuming finishing...\n");
	pthread_exit(NULL);
}
 
int main(int argc, char* argv[])   {
	int i;
	pthread_t producer_thread; 
	pthread_t consumer_thread[10];
	
	for (i = 0; i < 10; i++) {
		pthread_create(&consumer_thread[i], NULL, consumer, NULL);
	}
    pthread_create(&producer_thread, NULL, producer, NULL);
    for (i = 0; i < 10; i++) {
   		pthread_join(consumer_thread[i], NULL);
   	}
    return 0;
}


