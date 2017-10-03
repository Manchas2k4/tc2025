#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int i, val;
	
	val = fork();
	if (val == 0) {
		printf("PID = %i PPID = %i\n", getpid(), getppid());
	} else if (val > 0) {
		for (i = 0; i < 3; i++) {
			printf("i = % i - PID = %i PPID = %i\n", i, getpid(), getppid());
		}
	}
	return 0;
}
