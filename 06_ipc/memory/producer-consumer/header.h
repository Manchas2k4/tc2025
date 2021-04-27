#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define	OCCUPIED	0
#define FREESPACE	1
#define MUTEX		2

#define SIZE		10

struct buffer {
	int data[SIZE];
	int front, rear;
	int amount;
};

void enqueue(struct buffer *buffer, int val) {
	buffer->data[buffer->rear] = val;
	buffer->rear = (buffer->rear + 1) % SIZE;
	buffer->amount++;
}

int dequeue(struct buffer *buffer) {
	int val = buffer->data[buffer->front];
	buffer->data[buffer->front] = 0;
	buffer->front = (buffer->front + 1) % SIZE;
	buffer->amount--;
	return val;
}

void print_buffer(struct buffer *buffer) {
	int i, inf, sup;

	printf("amount = %i - front = %i - rear = %i\n",
		buffer->amount, buffer->front, buffer->rear);
	inf = MIN(buffer->front, buffer->rear);
	sup = MAX(buffer->front, buffer->rear);
	for (i = 0; i < SIZE; i++) {
		if (buffer->data[i] != 0) {
			printf("%3i", buffer->data[i]);
		} else {
			printf("   X");
		}
	}
	printf("\n");
}

int acquire(int semid, int sem_num, unsigned int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int release(int semid, int sem_num, unsigned int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

#endif
