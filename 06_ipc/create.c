#include "header.h"

int main(int argc, char* argv[]) {
	int chairs = 0, semid, shmid, i;
	unsigned short final_values[5];
	key_t key;

	if (argc != 2) {
		printf("usage: %s chairs\n", argv[0]);
		return -1;
	}

  chairs = atoi(argv[1]);
  if (chairs < 1) {
    printf("%s: the number of chairs must be greater than 0\n", argv[0]);
    return -1;
  }

	if ( (key = ftok("/dev/null", 250)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 5, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, CUSTOMER, SETVAL, 0);
	semctl(semid, BARBER, SETVAL, 0);
  semctl(semid, CUSTOMERDONE, SETVAL, 0);
  semctl(semid, BARBERDONE, SETVAL, 0);

	semctl(semid, 0, GETALL, final_values);
	printf("values: ");
	for (i = 0; i < 5; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");

	if ( (shmid = shmget(key, sizeof(SharedVariables), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	}

	SharedVariables *b;
	b = (SharedVariables*) shmat(shmid, (void*) 0, 0);
	b->n = chairs;
  b->customers = 0;
	shmdt(b);

	return 0;
}
