#include "header.h"

void a_consumer(char* program) {
	int i, k, semid;
	key_t key;
	
	if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	srand( getpid() );
	for (k = 0; k < 10; k++) {
		printf("Consumer %i trying to access the buffer.\n", getpid());
		sem_wait(semid, CONSUMERS, 1);
		printf("Consumer %i accesing the buffer.\n", getpid());
		
		i = (rand() % 5) + 1;
		printf("Consumer %i trying to get %i product(s) - items = %i\n", getpid(), i, semctl(semid, ITEMS, GETVAL, 0));
		sem_wait(semid, ITEMS, i);
		printf("Consumer %i took their products.\n", getpid());
		sem_signal(semid, SPACES, i);
		
		sem_signal(semid, CONSUMERS, 1);
		printf("Consumer %i is going to sleep.\n", getpid());
		
		sleep((rand() % 10) + 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount, pid, i;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s amount\n", argv[0]);
		return -1;
	}
	
	amount = atoi(argv[1]);
	if (amount < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_consumer(argv[0]);
		} else {
			//do_nothing
		}
	}
	
	return 0;
}
