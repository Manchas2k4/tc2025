#include "header.h"

void a_consumer() {
	int semid, shmid, i, j, k, size;
	key_t key;
	struct buffer *b;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, NUMSEM, 0666))  < 0 ) {
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
		printf("Consumer %i trying to access the buffer to get %i products.\n", getpid(), i);
		sem_wait(semid, FULL, i);
		
		printf("Consumer %i has accessed the buffer...\n", getpid());
		sem_wait(semid, MUTEX, 1);
		for (j = 0; j < i; j++) {
			printf("Consumer %i is taking the number %i\n", getpid(), b->data[b->next - 1]);
			b->next--;
		}
		printf("buffer = ");
		for (j = 0; j < SIZE; j++) {
			printf("%3i", b->data[j]);
		}
		printf("\n");
		printf("Consumer %i took their products.\n", getpid());
		sem_signal(semid, MUTEX, 1);
		
		sem_signal(semid, EMPTY, i);
		
		printf("Consumer %i is going to sleep.\n", getpid());
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
			a_consumer();
		} else {
		}
	}
	return 0;
}
