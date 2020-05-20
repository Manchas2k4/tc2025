#include "header.h"

void an_elf(int id) {
	key_t key;
	int semid, shmid, i, num;
	
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
	
	i = 0;
	while (i < 10) {
		num = rand() % 3 + 1;
		
		sleep(num);
		
		printf("Elf %i: I need help from Santa, I will wait for more elves to arrive.\n", id);
		sem_wait(semid, ELFTEX, 1);
		sem_wait(semid, MUTEX, 1);
		sh->elves++;
		if (sh->elves == 3) {
			printf("Elf %i: There are three of us, to wake up Santa.\n", id);
			sem_signal(semid, SANTASEM, 1);
		} else {
			printf("Elf %i: We are not three yet, I will wait.\n", id);
			sem_signal(semid, ELFTEX, 1);
		}
		sem_signal(semid, MUTEX, 1);
		
		sleep(5);
		printf("Elf %i: Santa is helping me.\n", id);
		
		sem_wait(semid, MUTEX, 1);
		sh->elves--;
		if (sh->elves == 0) {
			sem_signal(semid, ELFTEX, 1);
		}
		sem_signal(semid, MUTEX, 1);
		i++;
	}
	shmdt(sh);
	exit(0);
}

int main(int argc, char *argv[]) {
	int i, pid;
	
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < 7; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			an_elf(i);
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

	
