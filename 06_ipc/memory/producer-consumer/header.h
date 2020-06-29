#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	FULL 		0
#define EMPTY		1
#define MUTEX 		2

#define NUMSEM		3
#define SIZE		10

struct buffer {
	int next;
	int data[SIZE];
};

int sem_wait(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

#endif
