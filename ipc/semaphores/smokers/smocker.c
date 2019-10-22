#include "header.h"

char* names[3] = {"TOBACCO", "PAPER", "MATCH"};

int main(int argc, char* argv[]) {
	int semid, value;
	key_t key;
	
	if (argc != 2) {
		printf("usage: %s type\n", argv[0]);
		return -1;
	}
	
	value = atoi(argv[1]);
	if (value < 1 || value > 3) {
		printf("%s: the number must be between 1 and 3.\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	srand( getpid() );
	while (1) {
		sem_wait(semid, value, 1);
		
		printf("The smoker with %s take what the agent left, makes a cigar and smokes it..\n", names[value - 1]);
		
		sleep( rand() % 5 + 1 );
		
		sem_signal(semid, TABLE, 1);
	}

	return 0;
}
