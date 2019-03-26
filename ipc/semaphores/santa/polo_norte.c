#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define ELFOS 0
#define RENOS 1
#define SEM_SANTA 0
#define SEM_RENOS 1
#define SEM_ELFOS 2
#define SEM_MUTEX 3

int main(int argc, char* argv[]) {
	int shmid, semid, *arr;
	key_t llave;

	if ((llave = ftok("polo_norte.c", 'K')) == (key_t) -1) {
		perror("ftok");
		return -1;
	}
	if ((semid = semget(llave, 4, IPC_CREAT | 0600)) == -1) {
		perror("semget");
		return -1;
	}
	if ((shmid = shmget(llave, 2 * sizeof(int), IPC_CREAT | 0600)) == -1) {
		perror("shmget");
		return -1;
	}

	semctl(semid, SEM_SANTA, SETVAL, 0);
	semctl(semid, SEM_RENOS, SETVAL, 0);
	semctl(semid, SEM_ELFOS, SETVAL, 1);
	semctl(semid, SEM_MUTEX, SETVAL, 1);

	arr = (int *) shmat(shmid, 0, 0);
	arr[ELFOS] = 0;
	arr[RENOS] = 0;
	shmdt(arr);
	fprintf(stdout, "Recursos IPC creados...\n");
	return 0;
}
