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
	
	pid = fork();
	
	printf("PID = %i\tPPID = %i\n", getpid(), getppid());
	
	//printf("PID = %i\tPPID = %i - RESULT = %i PID = %i\n", getpid(), getppid(), WEXITSTATUS(result), rid);
	
	return 0;
}
