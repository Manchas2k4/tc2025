#include "header.h"

int main(int argc, char* argv[]) {
	int semid, value;
	key_t key;

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
		acquire(semid, TABLE, 1);

		value = (rand() % 3 ) + 1;
		switch (value) {
			case TOBACCO:
				printf("Agent is placing paper and match.\n");
				release(semid, TOBACCO, 1);
				break;
			case PAPER:
				printf("Agent is placing a tobacco and match.\n");
				release(semid, PAPER, 1);
				break;
			default:
				printf("Agent is placing a tobacco and paper.\n");
				release(semid, MATCH, 1);
				break;
		}

		sleep( rand() % 5 + 1 );
	}

	return 0;
}
