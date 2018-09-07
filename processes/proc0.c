#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

int main(int argc, char* argv[]) {
	int pid, result, rid;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		exit(-1);
	} else if (pid == 0) {
		// CHILD PROCESS
		sleep(2);
		printf("PID = %i\tPPID = %i\n", getpid(), getppid());
		exit(10);
	} else {
		// PARENT PROCESS
		rid = wait(&result);
		printf("PID = %i\tPPID = %i - RESULT = %i PID = %i\n", getpid(), getppid(), WEXITSTATUS(result), rid);
		exit(0);
	}
	return 0;
}
