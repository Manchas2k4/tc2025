#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

int find_minimum(char* program, char* file_in, int pos, int block) {
	int fd, num, min, i, *data;
	
	if ( (fd = open(file_in, O_RDONLY)) < 0 ) {
		printf("find_minimum: %s\n", file_in);
		perror(program);
		return -1;
	}
	
	data = (int *) malloc(block * sizeof(int));
	if (data == 0) {
		printf("%s: Not enough memory.\n", program);
		return -1;
	}
	lseek(fd, pos * block * sizeof(int), SEEK_SET);
	read(fd, data, block * sizeof(int));
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

void write_minimum(char* program, char* file_out, int pos, int minimum) {
	int fd;
	
	if ( (fd = open(file_out, O_WRONLY)) < 0 ) {
		printf("find_minimum: %s\n", file_out);
		perror(program);
		exit(-1);
	}

	lseek(fd, pos * sizeof(int), SEEK_SET);
	write(fd, &minimum, sizeof(int));
	close(fd);
}

void handler(int signum) {
	printf("PID = %i\tSIGNAL = %i.\n", getpid(), signum);
}

int child_process(char* program, char* file_in, char* file_out, int pos, int block) {
	int minimum;

	signal(SIGUSR2, handler);
	pause();
	printf("PID = %i finding minimum...\n", getpid());
	minimum = find_minimum(program, file_in, pos, block);
	printf("PID = %i minimum = %i.\n", getpid(), minimum);
	write_minimum(program, file_out, pos, minimum);
	printf("PID = %i ending...\n", getpid());
	exit(0);
}












