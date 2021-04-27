#include "header.h"

int main(int argc, char* argv[]) {
	int chairs = 0, semid, shmid, i;
	unsigned short final_values[5];
	key_t key;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

  if ( (key = ftok("/dev/null", 250)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 5, 0666))  < 0 ) {
		perror("semget");
		return -1;
	}

  srand(getpid());
  while(1) {
    printf("Barber: waiting for a client...\n");
    sem_wait(semid, CUSTOMER, 1);
    sem_signal(semid, BARBER, 1);

    printf("Barber: cuting the hair...\n");
    sleep((rand() % 3) + 1);
    printf("Barber: I finished...\n");

    sem_wait(semid, CUSTOMERDONE, 1);
    sem_signal(semid, BARBERDONE, 1);
    printf("Barber: another client?...\n");
  }
  return 0;
}
