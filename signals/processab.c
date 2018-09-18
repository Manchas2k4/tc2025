#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define MAX 100

int array[MAX], fd, i;

void handler_child(int signal) {
	for (i = 0; i < MAX; i++) {
		array[i] = rand();
	}
	
	if ( (fd = open("myfile", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror("child");
		exit(-1);
	}
	
	write(fd, array, sizeof(int) * MAX);
	
	close(fd);
}

void child() {
	signal(SIGUSR1, handler_child);
	
	srand(time(0));
	while(1) {
		pause();
		kill(getppid(), SIGUSR2);
	}	
		
	exit(0);
}

void handler_parent(int signal) {
	if ( (fd = open("myfile", O_RDONLY)) < 0 ) {
		perror("parent");
		exit(-1);
	}
	
	read(fd, array, sizeof(int) * MAX);
	
	double acum = 0;
	for (i = 0; i < MAX; i++) {
		acum += array[i];
	}
	acum /= MAX;
	
	printf("avg = %.3f\n", acum);
}

int main(int argc, char* argv[]) {
	int pid;
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		return -1;
	} else if (pid == 0) {
		child();
	} else {
		signal(SIGUSR2, handler_parent);
		sleep(2);
		
		while(1) {
			kill(pid, SIGUSR1);
			pause();
		}
	}
	return 0;
}
