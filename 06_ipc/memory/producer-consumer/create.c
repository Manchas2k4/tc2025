#include "header.h"

int main(int argc, char* argv[]) {
	int size = 0, semid, shmid, i;
	unsigned short final_values[3];
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

	semctl(semid, OCCUPIED, SETVAL, 0);
	semctl(semid, FREESPACE, SETVAL, SIZE);
	semctl(semid, MUTEX, SETVAL, 1);

	semctl(semid, 0, GETALL, final_values);
	printf("values: ");
	for (i = 0; i < 3; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");

	if ( (shmid = shmget(key, sizeof(Buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	}

	Buffer *b;
	b = (Buffer *) shmat(shmid, (void*) 0, 0);
	memset(b->data, 0, SIZE * sizeof(int));
	b->front = b->rear = b->amount = 0;
	print_buffer(b);
	shmdt(b);

	return 0;
}
