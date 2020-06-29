#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void handler(int sig) {
	printf("PID %i - SIGNAL - %i\n", getpid(), sig);
}

int main(int argc, char* argv[]) {
	signal(SIGTERM, handler);
	while(1) {
		pause();
	}
	exit(0);
}
