#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX 20
#define SEED 98765

#define FILENAME "dump.bin"

int *arr;
int current = 1;

void handler(int sig) {
	int fd;

	fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	write(fd, arr, sizeof(int) * current);
	close(fd);
	free(arr);

	// signal(SIGINT, SIG_DFL);
	exit(0);
}

int main(int argc, char* argv[]) {
	signal(SIGINT, handler);

	arr = (int*) malloc(sizeof(int) * MAX);
	arr[0] = SEED - 1;
	while(current < MAX) {
		arr[current] = (arr[current - 1] * 56789) % SEED;
		printf("current = %i - random = %i\n", current, arr[current]);
		current++;

		sleep(1);
	}
	free(arr);
	exit(0);
}

// kill -s SIGINT
