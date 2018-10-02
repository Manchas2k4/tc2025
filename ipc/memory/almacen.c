#include "semaforos.h"

int main(int argc, char* argv[]) {
	int tamanio = 0, semid, shmid, i, blk_size;;
	unsigned short valores_finales[NUMSEM];
	key_t key;
	
	if (argc != 1) {
		printf("forma de uso: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ( (semid = semget(key, NUMSEM, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}
	
	semctl(semid, LLENO, SETVAL, 0);
	semctl(semid, VACIO, SETVAL, SIZE);
	semctl(semid, MUTEX, SETVAL, 1);
	
	semctl(semid, 0, GETALL, valores_finales);
	printf("valores de los semaforos: ");
	for (i = 0; i < NUMSEM; i++) {
		printf("%3i", valores_finales[i]);
	}
	printf("\n");
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	} 	

	return 0;
}













