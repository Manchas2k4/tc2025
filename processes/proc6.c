#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

int main(int argc, char* argv[]) {
	int pid, result, rid;
	
	if (argc == 1) {
		fprintf(stderr, "usage: %s cmds\n", argv[0]);
		return -1;
	}
	
	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		exit(-1);
	} else if (pid == 0) {
		execvp(argv[1], &argv[1]);
	} else {
		wait(NULL);
	}
	return 0;
}
