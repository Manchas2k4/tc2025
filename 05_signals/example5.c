#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void handler1(int sig) {
	sigset_t unblock;

	printf("PID %i - SIGUSR1 going to sleep\n", getpid());
	sleep(10);
	printf("PID %i - SIGUSR1 has awakened\n", getpid());
	sigemptyset(&unblock);
	sigprocmask(SIG_SETMASK, &unblock, NULL);
}

void handler2(int sig) {
	sigset_t unblock;

	printf("PID %i - SIGUSR2 going to sleep\n", getpid());
	sleep(10);
	printf("PID %i - SIGUSR2 has awakened\n", getpid());
	sigemptyset(&unblock);
	sigprocmask(SIG_SETMASK, &unblock, NULL);
}

int main(int argc, char* argv[]) {
	int i;
	struct sigaction h1, h2;
	sigset_t suspended;

	sigfillset(&suspended);
	h1.sa_handler = handler1;
	h1.sa_mask = suspended;
	sigaction(SIGUSR1, &h1, NULL);

	h2.sa_handler = handler2;
	h2.sa_mask = suspended;
	sigaction(SIGUSR2, &h1, NULL);

	i = 0;
	while(1) {
		printf("i = %i\n", i);
		sleep(3);
		i++;
	}
	exit(0);
}

// kill -s SIGUSR1 407
// kill -s SIGUSR2 407
// kill 407
