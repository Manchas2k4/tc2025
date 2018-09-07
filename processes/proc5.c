#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

int main(int argc, char* argv[]) {
	int pid, result, rid;
	
	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		exit(-1);
	} else if (pid == 0) {
		execl("/bin/ls", "ls", "-l", "-R", "-a", (char*) 0);	
	} else {
		wait(NULL);
	}
	return 0;
}
