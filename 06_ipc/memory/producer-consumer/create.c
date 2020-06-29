#include "header.h"

int main(int argc, char* argv[]) {
	int size = 0, semid, shmid, i;
	unsigned short final_values[NUMSEM];
	key_t key;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ( (semid = semget(key, NUMSEM, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}
	
	semctl(semid, FULL, SETVAL, 0);
	semctl(semid, EMPTY, SETVAL, SIZE);
	semctl(semid, MUTEX, SETVAL, 1);
	
	semctl(semid, 0, GETALL, final_values);
	printf("values: ");
	for (i = 0; i < NUMSEM; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	} 	
	
	struct buffer *b;
	b = (struct buffer*) shmat(shmid, (void*) 0, 0);
	b->next = 0;
	memset(b->data, 0, SIZE * sizeof(int));
	shmdt(b);

	return 0;
}













