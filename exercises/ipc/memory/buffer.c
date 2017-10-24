#include "header.h"

int main(int argc, char* argv[]) {
	int semid, shmid, i;
	unsigned short final_values[3];
	key_t key;
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 3, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, EMPTY, SETVAL, SIZE);
	semctl(semid, FULL, SETVAL, 0);
	
	semctl(semid, 0, GETALL, final_values);
	for (i = 0; i < 3; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	
	if ( (shmid = shmget(key, sizeof(Buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror(argv[0]);
		return -1;
	}
	
	return 0;
}
