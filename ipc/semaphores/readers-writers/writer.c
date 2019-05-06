#include "header.h"

void process() {
	int i, semid;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 2, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	i = 0;
	while (i < 3) {
		printf("WRITER %i entering...\n",getpid());
		mutex_wait(semid, ROOMEMPTY);
		
		printf("WRITER %i working...\n",getpid());
		sleep(rand() % 5 + 1);
		
		mutex_signal(semid, ROOMEMPTY);
		printf("PID %i leaving...\n",getpid());
		
		i++;
	}
	
	exit(0);	
}

int main(int argc, char* argv[]) {
	int i, pid, num;
	
	if (argc != 2) {
		printf("usage: %s number\n", argv[0]);
		return -1;
	}
	
	num = atoi(argv[1]);
	if (num < 3) {
		printf("%s: the number must be greater than 3.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < num; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			process();
		} else {
		}
	}
	
	while (i > 0) {
		wait(NULL);
		i--;
	}
	
	return 0;
}
