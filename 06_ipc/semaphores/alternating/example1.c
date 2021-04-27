#include "header.h"

#define MAX 			20
#define PROCESSA	0
#define PROCESSB	1

void processA() {
	int i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 128)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 2, 0600)) < 0 ) {
		perror("semget");
		exit(-1);
	}

	for (i = 0; i < MAX; i++) {
		acquire(semid, PROCESSA, 1);
		printf("A PID %i - i = %i\n", getpid(), i);
		release(semid, PROCESSB, 1);
	}
	exit(0);
}

void processB() {
	int i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 128)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 2, 0600)) < 0 ) {
		perror("semget");
		exit(-1);
	}

	for (i = 0; i < MAX; i++) {
		acquire(semid, PROCESSB, 1);
		printf("B PID %i - i = %i\n", getpid(), i);
		release(semid, PROCESSA, 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int pid, i, semid;
	key_t key;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 128)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 2, IPC_CREAT | 0600)) < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, PROCESSA, SETVAL, 0);
	semctl(semid, PROCESSB, SETVAL, 1);

	for (i = 0; i < 2; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			switch (i) {
				case 0	: processA(); break;
				case 1	: processB(); break;
			}
		} else {
			// do nothing
		}
	}

	while (i > 0) {
		wait(NULL);
		i--;
	}

	semctl(semid, 0, IPC_RMID, NULL);
	return 0;
}
