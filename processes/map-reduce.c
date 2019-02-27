#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 1e6

typedef unsigned long int uli;

double aprox_pi(uli low, uli high) {
	double acum = 0;
	double aux = (low % 2 == 0)? 1 : -1;
	
	for (uli i = low; i < high; i++) {
	
		acum += aux / ((2 * i) + 1);
		aux *= -1;
	}
	return acum;
}

void child_process(int id, char *filename, uli low, uli high) {
	int fd;
	double result;
	
	printf("PID = %i PPID = %i - calculating from %lu to %lu\n", getpid(), getppid(), low, high);
	
	result = aprox_pi(low, high);
	
	if ( (fd = open(filename, O_WRONLY)) < 0 ) {
		perror("open");
		exit(-1);
	}
	
	lseek(fd, sizeof(double) * id, SEEK_SET);
	write(fd, &result, sizeof(double));
	close(fd);
	
	exit(0);
}

int main(int argc, char* argv[]) {
	int fd, i, pid, num;
	uli block, start, end;
	double *vals, acum;
	
	if (argc != 2) {
		printf("usage: %s number_of_childs\n", argv[0]);
		return -1;
	}
	
	num = atoi(argv[1]);
	if (num < 1) {
		printf("%s: number_of_childs must be a positive number greater than 0\n", argv[0]);
		return -1;
	}
	
	if ( (fd = open("temp", O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror("creating file");
		return -1;
	}
	close(fd);
	
	block = SIZE / num;
	printf("block size = %lu\n", block);
	
	for (i = 0; i < num; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			exit(-1);
		} else if (pid == 0) {
			start = (block) * i;
			end = (i != num-1)? (block * (i + 1)) : SIZE;
			child_process(i, "temp", start, end);
			exit(0);
		} else {
			//do nothing
		}
	}
	
	int rid;
	while (i > 0) {
		rid = wait(NULL);
		printf("The process %i has ended\n", rid);
		i--;
	} 
	
	if ( (fd = open("temp", O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	vals = (double*) malloc(sizeof(double) * num);
	
	read(fd, vals, sizeof(double) * num);
	close(fd);
	
	acum = 0;
	for (i = 0; i < num; i++) {
		acum += vals[i];
	}
	
	free(vals);


	printf("result = %lf\n", (acum * 4));
	
	remove("temp");
	return 0;
}
