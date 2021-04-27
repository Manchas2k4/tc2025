#include "header.h"

int main(int argc, char *argv[]) {
	key_t key;
	int semid, i, shmid, finished;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	if ((key = ftok("/dev/null", 117)) == (key_t) -1) {
		perror(argv[0]);
		return -1;
	}

	if ((semid = semget(key, 4, 0666)) < 0) {
		perror(argv[0]);
		return -1;
	}

	if ( (shmid = shmget(key, sizeof(SharedVars), 0666)) < 0 ) {
		perror("shmid");
		return -1;
	}

	SharedVars * sh;
	sh = (SharedVars*) shmat(shmid, (void*) 0, 0);

	finished = 0;
	while (!finished) {
		printf("Santa: I am going to sleep.\n");
		acquire(semid, SANTASEM, 1);
		printf("Santa: Oh! They woke me up, what is happening?\n");
		acquire(semid, MUTEX, 1);
		if (sh->reindeer >= 9) {
			printf("Santa: The nine reindeer arrived, prepare the sleigh.\n");
			release(semid, REINDEERSEM, 9);
			sh->reindeer -= 9;
			finished = 1;
		} else if (sh->elves == 3) {
			printf("Santa: Three elves need my help.\n");
			sleep(1);
			printf("Santa: Clever! Solved the elf problem. Now to continue sleeping..\n");
		}
		release(semid, MUTEX, 1);
	}

	shmdt(sh);

	semctl(semid, 0, IPC_RMID, 0);
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}
