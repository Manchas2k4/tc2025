#include "header.h"
#include <sys/wait.h>

void a_customer() {
  key_t key;
	int semid, shmid, k;
	SharedVariables *b;

  if ( (key = ftok("/dev/null", 250)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 5, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(SharedVariables), 0666)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		exit(-1);
	}

	b = (SharedVariables*) shmat(shmid, (void*) 0, 0);

	srand( getpid() );
	k = 10;
	while (k > 0) {
    printf("Customer %i: arriving to the barbershop\n", getpid());
    acquire(semid, MUTEX, 1);
    if (b->customers == b->n) {
      printf("Customer %i: It's full, I will return later\n", getpid());
      release(semid, MUTEX, 1);
    } else {
      b->customers++;
      release(semid, MUTEX, 1);

      printf("Customer %i: waiting for the barber\n", getpid());
      release(semid, CUSTOMER, 1);
      acquire(semid, BARBER, 1);

      printf("Customer %i: waiting...\n", getpid());
      sleep((rand() % 2) + 2);
      printf("Customer %i: done...\n", getpid());

      release(semid, CUSTOMERDONE, 1);
      acquire(semid, BARBERDONE, 1);

      acquire(semid, MUTEX, 1);
      b->customers--;
      release(semid, MUTEX, 1);

      k--;
      printf("Customer %i: I will return later for another cut\n", getpid());
    }
    sleep((rand() % 3) + 10);
	}

	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;

	if (argc != 2) {
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount <= 0) {
		printf("%s: The amount must be a positive number\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_customer();
		} else {
		}
	}

  while(i-- > 0) {
    wait(NULL);
  }
	return 0;
}
