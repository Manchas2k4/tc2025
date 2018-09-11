#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

void child_process(int level, char *program) {
	int pid, time;
	
	srand( getpid() );
	printf("LEVEL = %i\n", level);
	
	if (level == 1) {
		time = rand() % 5;
		printf("PID = %i PPID = %i SLEEP = %i\n", getpid(), getppid(), time);
		sleep( time );
		printf("PID = %i PPID = %i\n", getpid(), getppid());
		exit(0);
	} else {
		if ( (pid = fork()) < 0 ) {
			perror(program);
			exit(-1);
		} else if (pid == 0) {
			child_process(--level, program);
		} else {
			printf("PID = %i PPID = %i\n is waiting\n", getpid(), getppid());
			wait(NULL);
			printf("PID = %i PPID = %i\n has ended\n", getpid(), getppid());
			exit(0);
		}
	}
}

int main(int argc, char* argv[]) {
	int i, num, pid, result;
	
	if (argc != 2) {
		printf("usage: %s number_of_level\n", argv[0]);
		return -1;
	}
	
	num = atoi(argv[1]);
	if (num < 1) {
		printf("%s: number_of_level must be a positive number greater than 0\n", argv[0]);
		return -1;
	}
	
	child_process(num, argv[0]);
	return 0;
}
