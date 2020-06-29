#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define SIZE 3

void handler(int sig) {
	printf("PID %i - SIGNAL - %i\n", getpid(), sig);
}

void child_process() {
	signal(SIGUSR1, handler);
	while(1) {
		pause();
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int pids[SIZE], i;

	for (i = 0; i < SIZE; i++) {
		if ( (pids[i] = fork()) < 0 ) {
			perror("fork");
		} else if (pids[i] == 0) {
			child_process();
		} else {
			printf("PID %i created\n", pids[i]);
		}
	}

	sleep(1);

	for (i = 0; i < (SIZE * 2); i++) {
		printf("sending signal to PID %i\n", pids[i % 3]);
		kill(pids[i % 3], SIGUSR1);
	}

	sleep(8);

	for (i = 0; i < SIZE; i++) {
		kill(pids[i], SIGKILL);
	}

	return 0;
}
