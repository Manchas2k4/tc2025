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
	struct sigaction act1, act2;

	act1.sa_handler = handler1;
	sigemptyset(&act1.sa_mask);
	sigaddset(&act1.sa_mask, SIGUSR2);
	act1.sa_flags = 0;
	sigaction(SIGUSR1, &act1, 0);

	act2.sa_handler = handler2;
	sigemptyset(&act2.sa_mask);
	sigaddset(&act2.sa_mask, SIGUSR1);
	act2.sa_flags = 0;
	sigaction(SIGUSR2, &act2, 0);

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
