#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process(int level) {
	int time, pid;

	if (level == 1) {
		srand( getpid() );
		time = rand() % 5;
		printf("LEVEL %i - PPID = %i, PID = %i, SLEEP = %i\n", level, getppid(), getpid(), time);
		sleep(time);
		printf("LEVEL %i - PPID = %i, PID = %i\n", level, getppid(), getpid());
		exit(0);
	} else {
		if ( (pid = fork()) < 0 ) {
			perror("recursive fork");
			exit(-4);
		} else if (pid == 0) {
			child_process(--level);
		} else {
			printf("LEVEL = %i - PPID = %i PID = %i is waiting\n", level, getppid(), getpid());
			wait(NULL);
			printf("LEVEL = %i - PPID = %i PID = %i, child has ended\n", level, getppid(), getpid());
			exit(0);
		}
	}
}

int main(int argc, char* argv[]) {
	int pid, num, result, rid, i;

	if (argc != 2) {
		fprintf(stderr, "usage: %s level\n", argv[0]);
		return -1;
	}

	num = atoi(argv[1]);
	if (num <= 1) {
		fprintf(stderr, "%s: level must be a positive number greater than 1\n", argv[0]);
		return -2;
	}

	child_process(num);
	return 0;
}
