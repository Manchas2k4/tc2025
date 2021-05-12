#include "header.h"

int main(int argc, char* argv[]) {
	key_t key;
	int semid, i;
	unsigned short values[4];

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, AGENTSEM, SETVAL, 1);
	semctl(semid, TOBACCO, SETVAL, 0);
	semctl(semid, PAPER, SETVAL, 0);
	semctl(semid, MATCH, SETVAL, 0);

	semctl(semid, 0, GETALL, values);
	printf("values: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", values[i]);
	}
	printf("\n");
	return 0;
}
