#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIZE 100

/*********************** CHILD 1 ***************************/

void handler1_child1(int sig) {
	int i, fd, m[SIZE];
	
	srand(getpid());
	for (i = 0; i < SIZE; i++) {
		m[i] = rand() % 100;
	}
	
	if ( (fd = open("inter", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror("process1");
		exit(-1);
	}
	write(fd, m, sizeof(int) * SIZE);
	close(fd);
	printf("task ended - pid = %i ppid = %i\n", getpid(), getppid());
	
	kill(getppid(), SIGUSR1);
	printf("pid = %i sending sigusr1 to parent = %i\n", getpid(), getppid());
}

void handler2_child1(int sig) {
	printf("ending... pid = %i, getppid = %i\n", getpid(), getppid());
	exit(0);
}

void process1() {
	signal(SIGUSR1, handler1_child1);
	signal(SIGUSR2, handler2_child1);
	
	printf("pid %i has started...\n", getpid());
	while(1) {
		pause();
	}
	exit(0);
}	

/*********************** CHILD 1 ***************************/
/*********************** CHILD 2 ***************************/

void handler1_child2(int sig) {
	int i, fd, m[SIZE];
	
	if ( (fd = open("inter", O_RDONLY)) < 0 ) {
		perror("process2");
		exit(-1);
	}
	read(fd, m, sizeof(int) * SIZE);
	close(fd);
	
	double avg = 0;
	for (i = 0; i < SIZE; i++) {
		avg += m[i];
	}
	printf("avg = %lf - pid = %i ppid = %i\n", (avg/SIZE), getpid(), getppid());
	
	kill(getppid(), SIGUSR1);
	
	printf("pid = %i sending sigusr1 to parent = %i\n", getpid(), getppid());
}

void handler2_child2(int sig) {
	printf("ending... pid = %i, getppid = %i\n", getpid(), getppid());
	exit(0);
}

void process2() {
	signal(SIGUSR1, handler1_child2);
	signal(SIGUSR2, handler2_child2);
	
	printf("pid %i has started...\n", getpid());
	while(1) {
		pause();
	}
	exit(0);
}

/*********************** CHILD 2 ***************************/
/************************ PARENT ***************************/
int pids[2];
int turn = 0;
int count = 0;

void handler1_parent(int sig) {
	turn = (turn + 1) % 2;
	kill(pids[turn], SIGUSR1);	
	printf("sending sigusr1 to pids[%i] = %i\n", turn, pids[turn]);
	
	count++;
}

int main(int argc, char* argv[]) {
	int i, pid;
	
	signal(SIGUSR1, handler1_parent);
	
	for (i = 0; i < 2; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			if (i != 0) {
				kill(SIGKILL, pids[0]);
			}
			exit(0);
		} else if (pid == 0) {
			switch(i) {
				case 0 : process1(); break;
				case 1 : process2(); break;
			}
		} else {
			pids[i] = pid;
		}
		sleep(2);
	}

	turn = 0;	
	count = 0;
	kill(pids[turn], SIGUSR1);
	printf("sending sigusr1 to pids[%i] = %i\n", turn, pids[turn]);
	
	while(count < 30) {
		pause();
	}
	
	kill(pids[0], SIGUSR1);
	kill(pids[1], SIGUSR2);
	
	while(i > 0) {
		pid = wait(NULL);
		printf("pid %i ended\n", pid);
		i--;
	}
	
	return 0;
}
