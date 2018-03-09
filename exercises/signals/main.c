#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "child.h"

int main(int argc, char* argv[]) {
	int fd_out;
	int number_of_processes, block, *data, num, i, pid;
	struct stat buffer;

	if (argc != 3) {
		printf("usage: %s number_of_processes file_in.\n", argv[0]);
		return -1;
	}
	
	number_of_processes = atoi(argv[1]);
	if (number_of_processes < 2) {
		printf("%s: the number of processes must be greater than 1.\n", argv[0]);
		return -1;
	}
	
	if ( stat(argv[2], &buffer) < 0 ) {
		perror(argv[1]);
		return -1;
	}
	
	if ( (fd_out = open("inter", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		exit(-1);
	}
	close(fd_out);
	
	block = buffer.st_size / (number_of_processes * sizeof(int));
	data = (int*) malloc(number_of_processes * sizeof(int));
	if (data == 0) {
		printf("%s: Not enough memory.\n", argv[0]);
		return -1;
	}
	for (i = 0; i < number_of_processes; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			exit(-1);
		} else if (pid == 0) {
			child_process(argv[0], argv[2], "inter", i, block);
		} else {
			data[i] = pid;
		}
	}
	sleep(5);
	for (i = 0; i < number_of_processes; i++) {
		printf("PID = %i sending signal to CPID = %i...\n", getpid(), data[i]);
		kill(data[i], SIGUSR2);
	}
	while (i > 0) {
		wait(NULL);
		printf("PID = %i a child has ended\n", getpid());
		i--;
	}
	
	if ( (fd_out = open("inter", O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	read(fd_out, data, number_of_processes * sizeof(int));
	close(fd_out);
	
	int min = data[0];
	for (i = 0; i < number_of_processes; i++) {
		if (min > data[i]) {
			min = data[i];
		}
	}

	free(data);
	printf("PID = %i, RESULT = %i.\n", getpid(), min);
	remove("inter");
	return 0;
}	
	
