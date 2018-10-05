#include "header.h"

void a_producer() {
	int semid, shmid, i, j, k, blk_size, tamanio;
	key_t key;
	struct buffer *b;
	
	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	} 	
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);
		
	srand( getpid() );
	k = 10;
	while (k-- > 0) {
		i = (rand() % 5) + 1;
		printf("Producer %i entering the warehouse.\n", getpid());
		sem_wait(semid, EMPTY, i);
		
		printf("Producer %i is triying to put %i product(s)\n", getpid(), i);
		mutex_wait(semid, MUTEX);
		printf("Producer %i will place its products.\n", getpid());
		for (j = 0; j < i; j++) {
			b->data[b->tail] = (rand() % 3) + 1;
			printf("Producer %i has places the number %i.\n", getpid(), b->data[b->tail]);
			b->tail = (b->tail + 1) % SIZE;
		}
		printf("buffer = ");
		for (j = 0; j < SIZE; j++) {
			printf("%3i", b->data[j]);
		}
		printf("\n");
		printf("Producer %i placed its products.\n", getpid());
		mutex_signal(semid, MUTEX);
		
		sem_signal(semid, FULL, i);
		
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
		printf("%s: The amount must be a positive integer.\n", argv[0]);
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
	return 0;
}
