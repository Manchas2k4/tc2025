#include "header.h"

int main(int argc, char *argv[]) {
	key_t key;
	int semid, i, shmid;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	if ((key = ftok("/dev/null", 117)) == (key_t) -1) {
		perror(argv[0]);
		return -1;
	}

	if ((semid = semget(key, 4, 0666 | IPC_CREAT)) < 0) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, SANTASEM, SETVAL, 0);
	semctl(semid, REINDEERSEM, SETVAL, 0);
	semctl(semid, ELFTEX, SETVAL, 1);
	semctl(semid, MUTEX, SETVAL, 1);

	if ( (shmid = shmget(key, sizeof(SharedVars), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	} 	
	
	SharedVars * sh;
	sh = (SharedVars*) shmat(shmid, (void*) 0, 0);
	sh->reindeer = 0;
	sh->elves = 0;
	shmdt(sh);

	printf("Semaphore created...\n");
	return 0;
}

