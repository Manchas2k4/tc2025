#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TEMP "inter"

int main(int argc, char* argv[]) {
	int fd, number_of_processes, block, *data, num, i, pid;
	struct stat info;

	if (argc != 3) {
		fprintf(stderr, "usage: %s number_of_processes input_file\n", argv[0]);
		return -1;
	}

	number_of_processes = atoi(argv[1]);
	if (number_of_processes < 2) {
		fprintf(stderr, "%s: the number of processes must be greater than 1.\n", argv[0]);
		return -2;
	}

	if (stat(argv[2], &info) < 0) {
		perror("stat");
		return -3;
	}

	if ( (fd = open(TEMP, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror("open");
		return -4;
	}
	close(fd);

	block = info.st_size / (number_of_processes * sizeof(int));
	data = (int*) malloc(number_of_processes * sizeof(int));
	if (data == 0) {
		fprintf(stderr, "%s: Not enough memory.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < number_of_processes; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
		} else if (pid == 0) {
			child_process(argv[2], TEMP, i, block);
		} else {
			data[i] = pid;
		}
	}

	/*
	sleep(5);

	for (i = 0; i < number_of_processes; i++) {
		printf("PID %i sending signal to CPID %i...\n", getpid(), data[i]);
		kill(data[i], SIGUSR2);
	}
	*/
	
	while(i > 0) {
		wait(NULL);
		printf("PID %i, a child has ended.\n", getpid());
		i--;
	}

	fd = open(TEMP, O_RDONLY);
	read(fd, data, number_of_processes * sizeof(int));
	close(fd);

	int min = data[0];
	for (i = 0; i < number_of_processes; i++) {
		if (min > data[i]) {
			min = data[i];
		}
	}

	free(data);
	printf("PID %i, minimum = % i.\n", getpid(), min);
	remove("inter");

	return 0;
}
