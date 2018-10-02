#include "semaforos.h"
#include <time.h>

void un_productor() {
	int semid, shmid, i, j, k, blk_size, tamanio;
	key_t key;
	struct buffer *b;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, NUMSEM, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	} 	
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);
		
	srand( getpid() );
	k = 10;
	while (k-- > 0) {
		i = (rand() % 5) + 1;
		printf("productor %i entrando al almacen\n", getpid());
		sem_wait(semid, VACIO, i);
		
		printf("productor %i intentando colocar %i producto(s)\n", getpid(), i);
		mutex_wait(semid, MUTEX);
		printf("productor %i va a colocar los productos\n", getpid());
		for (j = 0; j < i; j++) {
			b->data[b->next++] = (rand() % 3) + 1;
			printf("productor %i colocando el numero %i\n", getpid(), b->data[b->next - 1]);
		}
		printf("buffer = ");
		for (j = 0; j < SIZE; j++) {
			printf("%3i", b->data[j]);
		}
		printf("\n");
		printf("productor %i coloco sus productos\n", getpid());
		mutex_signal(semid, MUTEX);
		
		sem_signal(semid, LLENO, i);
		
		sleep((rand() % 10) + 1);
	}
	
	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int cantidad = 0, semid, i, pid;
	key_t key;
	
	if (argc != 2) {
		printf("forma de uso: %s cantidad\n", argv[0]);
		return -1;
	}
	
	cantidad = atoi(argv[1]);
	if (cantidad <= 0) {
		printf("%s: La cantidad debe ser un entero positivo\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < cantidad; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			un_productor();
		} else {
		}
	}
	return 0;
}
