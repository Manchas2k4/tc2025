#include "header.h"

void process1() {
	int semid, shmid, i;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 1, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(int), 0666)) < 0 ) {
		perror("shmget");
		exit(-1);
	}
	
	int *b = (int *) shmat(shmid, (void *) 0, 0); 	
	(*b) = 0;
	
	i = 0;
	srand(time(0));
	while (i < N) {
		printf("+PID %i trying to get the semaphore\n", getpid());
		mutex_wait(semid, MUTEX);
		printf("+PID %i got the semaphore - old b = %i\n", getpid(), (*b));
		(*b)++;
		sleep(rand() % 5 + 1);
		mutex_signal(semid, MUTEX);
		printf("+PID %i release the semaphore\n", getpid());
		
		i++;
	}
	
	shmdt(b);
	exit(0);
}

void process2() {
	int semid, shmid, i;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 1, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(int), 0666)) < 0 ) {
		perror("shmget");
		exit(-1);
	}
	
	int *b = (int *) shmat(shmid, (void *) 0, 0); 	
	(*b) = 0;
	
	i = 0;
	srand(time(0));
	while (i < N) {
		printf("-PID %i trying to get the semaphore\n", getpid());
		mutex_wait(semid, MUTEX);
		printf("-PID %i got the semaphore - old b = %i\n", getpid(), (*b));
		(*b)--;
		sleep(rand() % 5 + 1);
		mutex_signal(semid, MUTEX);
		printf("-PID %i release the semaphore\n", getpid());
		
		i++;
	}
	
	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int i, pid, num;
	
	if (argc != 2) {
		printf("usage: %s number\n", argv[0]);
		return -1;
	}
	
	num = atoi(argv[1]);
	if (num < 3) {
		printf("%s: the number must be greater than 3.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < num; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			if (i % 2 == 0) {
				process1();
			} else {
				process2();
			}
		} else {
		}
	}
	
	while (i > 0) {
		wait(NULL);
		i--;
	}
	
	return 0;
}
