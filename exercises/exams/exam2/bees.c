#include "header.h"

void a_bee(int max, char* program) {
	int i, k, semid;
	int portions, current;
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
		portions = rand() % 3 + 1;
		
		printf("PID %i - Trying to put %i portions.\n", getpid(), portions);
		sem_wait(semid, MUTEX, 1);
		printf("PID %i - I am the only one, I will check the pot.\n", getpid());
		
		current = semctl(semid, SERVINGS, GETVAL, 0);
		printf("PID %i - The current portions in the pot are %i.\n", getpid(), current);
		
		if (current == max) {
			printf("PID %i - The pot is full, I'll wake the bear.\n", getpid());
			sem_signal(semid, FULLPOT, 1);
			sem_wait(semid, EMPTYPOT, 1);
		}
		
		if ((current + portions) > max) {
			printf("PID %i - There is no space. I'll just put %i portions.\n", getpid(), (max - current));
			sem_signal(semid, SERVINGS, (max - current));
		} else {
			printf("PID %i - There is space. I'll will put all my portions.\n", getpid());
			sem_signal(semid, SERVINGS, portions);
		}
		printf("PID %i - I'm going so another bee came in.\n", getpid());
		sem_signal(semid, MUTEX, 1);
				 
		sleep((rand() % 10) + 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount, size, pid, i;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s amount_bees size\n", argv[0]);
		return -1;
	}
	
	amount = atoi(argv[1]);
	if (amount < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	size = atoi(argv[1]);
	if (size < 1) {
		fprintf(stderr, "%s: The size must be a positive number greater than zero.\n", argv[1]);
		return -1;
	}
	
	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_bee(size, argv[0]);
		} else {
			//do_nothing
		}
	}
	
	return 0;
}
