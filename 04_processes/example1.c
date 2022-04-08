#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int pid, i;

	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork");
		return -2;
	} else if (pid == 0) {
		for (i = 0; i < 1000; i++) {
			printf("id = %i - CHILD PROCESS <-> i = %i\n", getpid(), i);
		}
		exit(10);
	} else {
		int result, rid;
		for (i = 0; i < 1000; i++) {
			printf("id = %i - PARENT PROCESS <-> i = %i\n", getpid(), i);
		}
		rid = wait(&result);
		printf("id = %i - PARENT PROCESS\n", getpid());
		printf("child id = %i, result = %i\n", rid, WEXITSTATUS(result));
	}
	return 0;
}
