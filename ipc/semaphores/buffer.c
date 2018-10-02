#include "header.h"

int main(int argc, char* argv[]) {
	int size = 0, semid, i;
	unsigned short final_values[4];
	key_t key;
	
	if (argc != 2) {
		printf("usage: %s size\n", argv[0]);
		return -1;
	}
	
	size = atoi(argv[1]);
	if (size < 1) {
		printf("%s: The size must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 4, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	semctl(semid, PRODUCER, SETVAL, 1);
	semctl(semid, CONSUMER, SETVAL, 1);
	semctl(semid, OCCUPIED, SETVAL, 0);
	semctl(semid, FREESPACE, SETVAL, size);
	
	semctl(semid, 0, GETALL, final_values);
	printf("values: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	return 0;
}
