#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define SIZE 1000

/*********************** CHILD0 ****************************/
void handler_child0(int sig) {
	printf("PID = %i - SIGNAL %i received\n", getpid(), sig);
}

void write_number(char* prefix, int iter, char* program) {
	int fd, i, num, *data;
	char filename[NAME_MAX + 1];
	
	srand( 12345 );
	data = (int*) malloc(SIZE * sizeof(int));
	for (i = 0; i < SIZE; i++) {
		data[i] = (rand() %  100) + 1;
		
	}
	
	sprintf(filename, "%s0%i.dat", prefix, iter);
	if ( (fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(program);
		return;
	}
	write(fd, data, SIZE * sizeof(int));
	close(fd);
	
	
	free(data);
}

void child0(char* prefix, char* program) {
	int i;
	
	signal(SIGUSR1, handler_child0);
	i = 0;
	while (i < 3) {
		pause();
		write_number(prefix, i, program);
		kill(getppid(), SIGUSR1);
		i++;
	}
	exit(0);
}

/*********************** CHILD1 ****************************/
void handler_child1(int sig) {
	printf("PID = %i - SIGNAL %i received\n", getpid(), sig);
}

void child1(char* in, char* out, char* program) {
	int i;
	
	signal(SIGUSR2, handler_child1);
	i = 0;
	while (i < 3) {
		pause();
		printf("PID = %i - DOING MY TASK\n", getpid());
		kill(getppid(), SIGUSR2);
		i++;
	}
	exit(0);
}

/*********************** PARENT ****************************/
void handler(int sig) {
	printf("PID = %i - SIGNAL %i received\n", getpid(), sig);
}

int main(int argc, char* argv[]) {
	int timeout, pids[2], i;
	char *inprefix, *outprefix;
	
	if (argc != 4) {
		fprintf(stderr, "Usage: %s timeout in_prefix out_prefix\n", argv[0]);
		return -1;
	}
	
	timeout = atoi(argv[1]);
	if (timeout < 1) {
		fprintf(stderr, "%s: The timeout must be a positive integer\n", argv[0]);
		return -1;
	}
	
	inprefix = argv[2];
	outprefix = argv[3];
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	for (i = 0; i < 2; i++) {
		if ( (pids[i] = fork()) < 0 ) {
			perror(argv[0]);
			exit(-1);
		} else if (pids[i] == 0) {
			switch (i) {
				case 0 : child0(inprefix, argv[0]); break;
				case 1 : child1(inprefix, outprefix, argv[0]); break;
			}
		} else {
			// do_nothing
		}
	}
	
	sleep(1);
	int cont = 0;
	while (cont < 3) {
		kill(pids[0], SIGUSR1);
		pause();
		kill(pids[1], SIGUSR2);
		pause();
		cont++;
	}
	
	printf("PID = %i waiting...\n", getpid());
	while (i > 0) {
		wait(NULL);
		i--;
	}
	
	return 0;
}
