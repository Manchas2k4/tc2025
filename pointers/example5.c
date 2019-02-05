/**
	This program defines a ADT Queue using structures 
	and dynamic arrays of C.
	
	@author Pedro Perez
	@version 1.0 13/01/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

typedef unsigned int uint;
typedef long int li;

struct a_queue{
	uint *data;
	uint size, elements, front, rear;
};

typedef struct a_queue queue;

queue* create_queue(uint size) {
	if (size == 0) {
		return NULL;
	}
	
	queue* q = (queue*) malloc(sizeof(queue));
	if (q == NULL) {
		return NULL;
	}
	
	q->data = (uint*) malloc(sizeof(uint) * size);
	if (q->data == NULL) {
		return NULL;
	}
	
	q->size = size;
	q->elements = 0;
	q->front = 0;
	q->rear = 0;
	return q;
}

int enqueue(queue *q, uint val) {
	if (q == NULL) {
		return -1;
	}
	
	if (q->data == NULL) {
		return -1;
	}
	
	if (q->elements == q->size) {
		return -1;
	}
	
	q->data[q->rear] = val;
	q->rear = (q->rear + 1) % q->size;
	q->elements++;
	return 1;
}

li dequeue(queue *q) {
	uint val;
	
	if (q == NULL) {
		return -1;
	}
	
	if (q->data == NULL) {
		return -1;
	}
	
	if (q->elements == 0) {
		return -1;
	}
	
	val = q->data[q->front];
	q->front = (q->front + 1) % q->size;
	q->elements--;
	return val;
}

int empty(queue *q) {
	if (q == NULL) {
		return -1;
	}
	
	if (q->data == NULL) {
		return -1;
	}
	
	return (q->elements == 0);
}

int free_queue(queue *q) {
	free(q->data);
	free(q);
	q = 0;
}

int display(queue *q) {
	uint i;
	
	if (q == 0) {
		return -1;
	}
	
	if (q->data == 0) {
		return -1;
	}
	
	i = q->front;
	while (i != q->rear) {
		printf("%3i", q->data[i]);
		i = (i + 1) % q->size;
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	int i;
	queue *q = create_queue(20);
	
	if (q == NULL) {
		printf("ERROR\n");
		return -1;
	}
	
	for (i = 0; i < N; i++) {
		enqueue(q, i);
	}	
	display(q);
	
	while(!empty(q)) {
		li val = dequeue(q);
		
		if (val < 0) {
			printf("ERROR\n");
			free_queue(q);
			return -1;
		}
		
		printf("val = %li\n", val);
	}
	
	free_queue(q);
	return 0;
}


