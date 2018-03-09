#include "header.h"

int main(int argc, char* argv[]) {
	int semid, i, num;
	key_t key;
	
	if ( (key = ftok("/dev/null", 99)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 2, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	i = 0;
	srand(getpid());
	while (i < 15) {
		sem_wait(semid, PROCESS_1, 1);
		
		num = rand() % 3 + 1;
		printf("PROCESS 1 - PID = %i - SLEEP = % i - i = %i\n", getpid(), num, i); 
		
		sem_signal(semid, PROCESS_2, 1);
		sleep(num);
		i++;
	}
	
	semctl(semid, IPC_RMID, 0);
	return 0;
}
