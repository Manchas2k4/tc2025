#include "header.h"

int main(int argc, char* argv[]) {
	int i, semid, size;
	unsigned short final_values[4];
	key_t key;
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
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
	
	semctl(semid, SERVINGS, SETVAL, 0);
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, EMPTYPOT,	 SETVAL, 0);
	semctl(semid, FULLPOT,	 SETVAL, 0);
	
	semctl(semid, 0, GETALL, final_values);
	printf("value: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	return 0;
}
