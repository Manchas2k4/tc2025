#include "header.h"

#define PARENT	0
#define CHILD 	1
#define N 		5

void child_process() {
	int i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 127)) < 0 ){
		perror("ftok");
		exit(1);
	}

	if ( (semid = semget(key, 2, 0666)) < 0 ) {
		perror("semget");
		exit(-1);
	}

	for (i = 0; i < N; i++) {
		sem_wait(semid, CHILD, 1);
		printf("CHILD %i\n", getpid());
		sleep(1);
		sem_signal(semid, PARENT, 1);
	}
	exit(0);
}

void parent_process() {
	int i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 127)) < 0 ){
		perror("ftok");
		exit(-1);
	}
	if ( (semid = semget(key, 2, 0666)) < 0 ) {
		perror("semget");
		exit(-1);
	}
	for (i = 0; i < N; i++) {
		sem_wait(semid, PARENT, 1);
		printf("PARENT %i \n", getpid());
		sleep(1);
		sem_signal(semid, CHILD, 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int pid, i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 127)) < 0 ){
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 2, 0666 | IPC_CREAT)) < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, PARENT, SETVAL, 0);
	semctl(semid, CHILD, SETVAL, 1);

	if ( (pid = fork()) < 0 ) {
		perror("fork");
	} else if (pid == 0) {
		child_process();
	} else {
		parent_process();
		wait(NULL);
	}
	return 0;
}
