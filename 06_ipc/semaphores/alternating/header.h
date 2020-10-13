#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int sem_wait(int semid, int sem_num, unsigned int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0; // IPC_NOWAIT;
	return semop(semid, &op, 1);
}

/*
	struct sembuf op[2];
	op[0].sem_num = 1;
	op[0].sem_op = -val;
	op[0].sem_flg = 0; // IPC_NOWAIT;
	
	op[1].sem_num = 0;
	op[1].sem_op = val;
	op[1].sem_flg = 0; 
	semop(semid, op, 2);
*/

int sem_signal(int semid, int sem_num, unsigned int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

#endif
