#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <signal.h>

int find_minimum(char* input_file, int wid, int block) {
	int fd, num, min, i, *data;

	data = (int*) malloc(block * sizeof(int));
	if (data == 0) {
		fprintf(stderr, "not enough memory.\n");
		return -1;
	}

	if ( (fd = open(input_file, O_RDONLY)) < 0 ) {
		perror("open find minimum");
		return -1;
	}

	lseek(fd, wid * block * sizeof(int), SEEK_SET);
	read(fd, data, block *sizeof(int));
	close(fd);

	min = INT_MAX;
	for (i = 0; i < block; i++) {
		if (min > data[i]) {
			min = data[i];
		}
	}

	free(data);
	data = 0;
	return min;
}

int write_minimum(char* output_file, int wid, int minimum) {
	int fd;

	if ( (fd = open(output_file, O_WRONLY)) < 0 ) {
		perror("open write minimum");
		return -1;
	}
	lseek(fd, wid * sizeof(int), SEEK_SET);
	write(fd, &minimum, sizeof(int));
	close(fd);
	return 0;
}

void handler(int sig) {
	printf("PID %i started...\n", getpid());
}

void child_process(char* input_file, char* output_file, int wid, int block) {
	int minimum, aux;

	signal(SIGUSR2, handler);
	pause();
	printf("PID %i finding minimum...\n", getpid());
	minimum = find_minimum(input_file, wid, block);
	if (minimum < 0) {
		minimum = INT_MAX;
	}
	printf("PID %i writing minimum...\n", getpid());
	aux = write_minimum(output_file, wid, minimum);
	printf("PID %i ending...\n", getpid());
	exit(aux);
}
