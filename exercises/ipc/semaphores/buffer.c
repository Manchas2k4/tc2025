#include "header.h"

int main(int argc, char* argv[]) {
	int i, semid, size;
	unsigned short final_values[4];
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
	
	semctl(semid, PRODUCERS, SETVAL, 1);
	semctl(semid, CONSUMERS, SETVAL, 1);
	semctl(semid, ITEMS,	 SETVAL, 0);
	semctl(semid, SPACES,	 SETVAL, size);
	
	semctl(semid, PRODUCERS, GETALL, final_values);
	printf("value: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	return 0;
}
