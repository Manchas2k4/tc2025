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
		sem_wait(semid, TABLE, 1);

		value = (rand() % 3 ) + 1;
		switch (value) {
			case TWENTY:
				printf("Benji is placing a 50 bill and a 100 bill.\n");
				sem_signal(semid, TWENTY, 1);
				break;
			case FIFTY:
				printf("Benji is placing a 20 bill and a 100 bill.\n");
				sem_signal(semid, FIFTY, 1);
				break;
			default:
				printf("Benji is placing a 20 bill and a 50 bill.\n");
				sem_signal(semid, HUNDRED, 1);
				break;
		}
		
		sleep( rand() % 5 + 1 );
	}

	return 0;
}