#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <limits.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h> 

#define SIZE 1000
#define TIMES 3
#define CHILDS 2

struct message {
	long type;
	int data[SIZE];
};

void process_one() {
	struct message msg;
	int i, j, msqid;
	long length = sizeof(struct message) - sizeof(long);
	key_t key;
	
	if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror("one");
        exit(-1);
    }
    
    if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror("one");
        exit(-1);
    }
	
	msg.type = 1;
	for (j = 0; j < TIMES; j++) {
		printf("process one is sleeping...\n");
		sleep(1);
		printf("process one is awakening...\n");
		
		for (i = 0; i < SIZE; i++) {
			msg.data[i] = (rand() % 100) + 1;
		}
	
		msgsnd(msqid, &msg, length, 0);
	}
	
	exit(0);
}

void process_two() {
	float avg, sd;
	struct message msg;
	int i, j, msqid;
	long length = sizeof(struct message) - sizeof(long);
	key_t key;
	
	if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror("two");
        exit(-1);
    }
    
    if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror("two");
        exit(-1);
    }
    
    for (j = 0; j < TIMES; j++) {
		msgrcv(msqid, &msg, length, 1, 0);
		printf("process two has read 1000 numbers\n");
		
		avg = 0;
		for (i = 0; i < SIZE; i++) {
			avg += msg.data[i];
		}
		avg = avg / (float) SIZE;
	
		sd = 0;
		for (i = 0; i < SIZE; i++) {
			sd += (msg.data[i] - avg);
		}
		sd = sqrt( sd / (float) SIZE );
	
		printf("AVG = %f SD = %f\n", avg, sd);
	}
	
	exit(0);
}

int main(int argc, char* argv[]) {
	int i, pid, msqid;
	key_t key;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    if ( (msqid = msgget(key, 0666 | IPC_CREAT)) == -1 ) {
        perror(argv[0]);
        return -1;
    }
	
	for (i = 0; i < CHILDS; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			exit(-1);
		} else if (pid == 0) {
			switch (i) {
			case 0 : process_one(); break;
			case 1 : process_two(); break;
			}
		} else {
			printf("The process %i has been created\n", pid);
		}
	}
	
	while (i > 0) {
		wait(NULL);
		i--;
	}
	
	return 0;
}
	
