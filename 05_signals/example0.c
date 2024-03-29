#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

/*
void handler(int sig) {
	switch(sig) {
		case SIGUSR1 : printf("PID %i - SIGUSR1\n", getpid()); break;
		case SIGUSR2 : printf("PID %i - SIGUSR2\n", getpid()); break;
	}
}
*/

void handler1(int sig) {
	printf("PID %i - SIGUSR1 going to sleep\n", getpid());
	sleep(10);
	printf("PID %i - SIGUSR1 has awakened\n", getpid());
}

void handler2(int sig) {
	printf("PID %i - SIGUSR2 going to sleep\n", getpid());
	sleep(10);
	printf("PID %i - SIGUSR2 has awakened\n", getpid());
}

int main(int argc, char* argv[]) {
	int i;

	/*
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	*/

	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler2);

	i = 0;
	while(1) {
		printf("i = %i\n", i);
		sleep(30);
		i++;
	}
	exit(0);
}

// kill -s SIGUSR1 426
// kill -s SIGUSR2 426
// kill 426
