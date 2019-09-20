#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int pid;

	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork");
		return -2;
	} else if (pid == 0) {
		printf("id = %i - CHILD PROCESS\n", getpid());
		sleep(2);
		exit(10);
	} else {
		int result, rid;

		rid = wait(&result);
		printf("id = %i - PARENT PROCESS\n", getpid());
		printf("child id = %i, result = %i\n", rid, WEXITSTATUS(result));
	}
	return 0;
}
