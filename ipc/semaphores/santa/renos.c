#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

void acquiere(int semid, int sem, int val) {
	struct sembuf operacion;

	operacion.sem_num = sem;
	operacion.sem_op = -val;
	operacion.sem_flg = 0;
	semop(semid, &operacion, 1);
}

void release(int semid, int sem, int val) {
	struct sembuf operacion;

	operacion.sem_num = sem;
	operacion.sem_op = val;
	operacion.sem_flg = 0;
	semop(semid, &operacion, 1);
}

void reno(int id) {
	int shmid, semid, *arr, finalizado, val;
	key_t llave;

	if ((llave = ftok("polo_norte.c", 'K')) == (key_t) -1) {
		perror("ftok");
		exit(-1);
	}
	if ((semid = semget(llave, 4, 0600)) == -1) {
		perror("semget");
		exit(-1);
	}
	if ((shmid = shmget(llave, 2 * sizeof(int), 0600)) == -1) {
		perror("shmget");
		exit(-1);
	}

	arr = (int *) shmat(shmid, 0, 0);

	srand(time(0));
	finalizado = 0;
	while (!finalizado) {
		val = (rand() % 5) + 1;
		sleep(val);

		/* sem_mutex.acquiere() */
		acquiere(semid, SEM_MUTEX, 1);
		arr[RENOS]++;
		fprintf(stdout, "reno %i ha llegado\n", id);
		if (arr[RENOS] == 9) {
			/* sem_santa.release() */
			release(semid, SEM_SANTA, 1);
		}
		/* sem_mutex.release() */
		release(semid, SEM_MUTEX, 1);

		/* sem_reno.acquiere() */
		acquiere(semid, SEM_RENOS, 1);
		fprintf(stdout, "reno %i ya esta listo para salir\n", id);
		finalizado = 1;
	}
	exit(0);
}


int main(int argc, char* argv[]) {
	int pid, i;

	for (i = 1; i <= 9; i++) {
		if ((pid = fork()) < 0) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			reno(i);
		}
	}
	while (i > 0) {
		wait(0);
		i--;
	}
	return 0;
}
