#include "header.h"

void a_reindeer(int id) {
	key_t key;
	int semid, i, shmid;
	
	if ((key = ftok("/dev/null", 117)) == (key_t) -1) {
		perror("ftok");
		exit(-1);
	}

	if ((semid = semget(key, 4, 0666)) < 0) {
		perror("semid");
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(SharedVars), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	} 
	
	SharedVars * sh;
	sh = (SharedVars*) shmat(shmid, (void*) 0, 0);
	
	srand( getpid() );
	
	i = rand() % 5 + 1;
	printf("Reindeer %i: I am going on vacation, I am coming back in %i seconds.\n", id, i);
	sleep( i );
	
	printf("Reindeer %i: I am back.\n", id);
	sem_wait(semid, MUTEX, 1);
	sh->reindeer++;
	if (sh->reindeer == 9) {
		sem_signal(semid, SANTASEM, 1);
	}
	sem_signal(semid, MUTEX, 1);
	
	sem_wait(semid, REINDEERSEM, 1);
	printf("Reindeer %i: Ready to rumble...\n", id);
	
	shmdt(sh);
	exit(0);
}

int main(int argc, char *argv[]) {
	int i, pid;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < 9; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			a_reindeer(i);
		} else {
			// do nothing
		}
	}
	
	while (i > 0) {
		wait(NULL);
		i--;
	}
	
	return 0;
}

	
