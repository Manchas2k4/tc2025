#include "header.h"

int main(int argc, char* argv[]) {
	int semid, shmid, i;
	unsigned short initial_values[3];
	key_t key;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ( (semid = semget(key, 3, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}
	
	semctl(semid, FULL, SETVAL, 0);
	semctl(semid, EMPTY, SETVAL, SIZE);
	semctl(semid, MUTEX, SETVAL, 1);
	
	semctl(semid, 0, GETALL, initial_values);
	printf("initial_values: ");
	for (i = 0; i < 3; i++) {
		printf("%3i", initial_values[i]);
	}
	printf("\n");
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}
	
	struct buffer *b = (struct buffer *) shmat(shmid, (void *) 0, 0); 	
	b->head = 0;
	b->tail = 0;
	shmdt(b);

	return 0;
}













