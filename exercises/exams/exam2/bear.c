#include "header.h"

int main(int argc, char* argv[]) {
	int semid, size;
	key_t key;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s size\n", argv[0]);
		return -1;
	}
	
	size = atoi(argv[1]);
	if (size < 1) {
		fprintf(stderr, "%s: The size must be a positive number greater than zero.\n", argv[1]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	while (1) {
		printf("\tPID = %i - ZZzzzzZZZZ\n", getpid());
		sem_wait(semid, FULLPOT, 1);
		printf("\tPID %i - The pot is full, let's eat!\n", getpid());
		sem_wait(semid, SERVINGS, size);
		sem_signal(semid, EMPTYPOT, 1);
		printf("\tPID %i - While they refill the pot, I go to sleep.\n", getpid());
		sleep(3);
	}
	
	return 0;
}
