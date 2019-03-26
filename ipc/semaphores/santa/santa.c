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

void wait(int semid, int sem, int val) {
	struct sembuf operacion;

	operacion.sem_num = sem;
	operacion.sem_op = -val;
	operacion.sem_flg = 0;
	semop(semid, &operacion, 1);
}

void signal(int semid, int sem, int val) {
	struct sembuf operacion;

	operacion.sem_num = sem;
	operacion.sem_op = val;
	operacion.sem_flg = 0;
	semop(semid, &operacion, 1);
}


int main(int argc, char* argv[]) {
	int shmid, semid, *arr, finalizado;
	key_t llave;

	if ((llave = ftok("polo_norte.c", 'K')) == (key_t) -1) {
		perror("ftok");
		return -1;
	}
	if ((semid = semget(llave, 4, 0600)) == -1) {
		perror("semget");
		return -1;
	}
	if ((shmid = shmget(llave, 2 * sizeof(int), 0600)) == -1) {
		perror("shmget");
		return -1;
	}

	arr = (int *) shmat(shmid, 0, 0);
	
	finalizado = 0;
	while (!finalizado) {
		/* sem_santa.wait() */
		wait(semid, SEM_SANTA, 1);

		/* sem_mutex.wait() */
		wait(semid, SEM_MUTEX, 1);
		fprintf(stdout, "Cuantos renos hay? %i\n", arr[RENOS]);
		fprintf(stdout, "Cuantos elfos hay? %i\n", arr[ELFOS]);
		if (arr[RENOS] == 9) {
			fprintf(stdout, "Ya llegaron los renos... ha preparar el trineo !!!\n");
			/* sem_renos.signal() */
			signal(semid, SEM_RENOS, 9);
			finalizado = 1;
		} else if (arr[ELFOS] == 3) {
			fprintf(stdout, "Bueno... hay que ayudar a estos elfos\n");
		}
		/* sem_mutex.wait() */
		signal(semid, SEM_MUTEX, 1);
	}
	fprintf(stdout, "Vamos a repartir regalos!!! JOJOJO!!\n");
	return 0;
}
