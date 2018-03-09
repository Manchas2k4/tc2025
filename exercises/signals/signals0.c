#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig) {
	printf("PID %i - handling the signal %i\n", getpid(), sig);
}

int main(int argc, char* argv[]) {
	unsigned int result;
	
	if (signal(SIGUSR1, handler) == SIG_ERR) {
		perror(argv[0]);
		return -1;
	}
	
	int i = 0;
	while (1) {
		printf("PID %i - i = %i\n", getpid(), i);
		result = sleep(10);
		printf("result = %u\n", result);
		i++;
	}
	return 0;
}
