#include "header.h"

int main(int argc, char* argv[]) {
	int semid, i;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		exit(-1);
	}
	
	if ( (semid = semget(key, 2, 0666))  < 0 ) {
		perror(argv[0]);
		exit(-1);
	}
	
	i = 0;
	while (i < N) {
		mutex_wait(semid, PROCESS2);
		printf("ID = %i - i = %i\n", getpid(), i);
		mutex_signal(semid, PROCESS1);
		i++;
	}
}
