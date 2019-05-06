#include "header.h"

int main(int argc, char* argv[]) {
	int semid, shmid;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 2, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, ROOMEMPTY, SETVAL, 1);
	
	if ( (shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}
	
	int *b = (int *) shmat(shmid, (void *) 0, 0); 	
	(*b) = 0;
	shmdt(b);	

	return 0;
}
