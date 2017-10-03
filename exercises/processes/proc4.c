#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


void child_process(int level, char* program) {
	int pid;
	
	if (level == 1) {
		printf("%s: PID = %i\tPPID = %i\n", program, getpid(), getppid());
		sleep(2);
		printf("%s: PID = %i has ended\n", program, getpid());	
		exit(0);
	} else {
		if ( (pid = fork()) < 0 ) {
			perror(program);
			exit(-1);
		} else if (pid == 0) {
			child_process(--level, program);
		} else {
			printf("%s: PID = %i\tPPID = %i\n", program, getpid(), getppid());
			wait(NULL);
			printf("%s: PID = %i has ended\n", program, getpid());
			exit(0);
		}
	}
}

int main(int argc, char* argv[]) {
	int pid, i, level;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s level\n", argv[0]);
		return -1;
	}
	
	level = atoi(argv[1]);
	if (level <= 0) {
		fprintf(stderr, "%s: the number must be a positive greater than 0\n", argv[0]);
		return -1;
	}
	
	child_process(level, argv[0]);
	return 0;
}
