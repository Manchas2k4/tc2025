#include "header.h"

void a_producer() {
	int semid, shmid, i, j, k, size, aux;
	key_t key;
	Buffer *b;

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	b = (Buffer *) shmat(shmid, (void *) 0, 0);

	srand( getpid() );
	k = 10;
	while (k-- > 0) {
		i = (rand() % 5) + 1;
		printf("Producer %i trying to access the buffer to put %i product(s).\n", getpid(), i);
		acquire(semid, FREESPACE, i);

		printf("Producer %i has accessed the buffer...\n", getpid());
		acquire(semid, MUTEX, 1);
		for (j = 0; j < i; j++) {
			aux = (rand() % 3) + 1;
			enqueue(b, aux);
			printf("Producer %i is putting the number %i\n", getpid(), aux);
		}
		print_buffer(b);

		printf("Producer %i put their products.\n", getpid());
		release(semid, MUTEX, 1);

		release(semid, OCCUPIED, i);

		printf("Producer %i is going to sleep.\n", getpid());
		sleep((rand() % 10) + 1);
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
			a_producer();
		} else {
		}
	}

	while (i > 0) {
		wait(NULL);
		i--;
	}
	return 0;
}
