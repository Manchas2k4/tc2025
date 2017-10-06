#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int pid;
	
	if (argc < 2) {
		fprintf(stderr, "usage: %s cmd1 cmd2 ... cmdn\n", argv[0]);
		return -1;
	}
	
	printf("PARENT PID = %i\n", getpid());
	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		return -1;
	} else if (pid == 0) {
		printf("CHILD PID = %i\n", getpid());
		execvp(argv[1], &argv[1]);
	} else {
		wait(NULL);
	}
	
	printf("PARENT PID = %i ENDED\n", getpid());
	return 0;
}
