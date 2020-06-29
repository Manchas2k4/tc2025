#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <limits.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h> 
#include <fcntl.h>

#define SIZE 10000000
#define GRAIN 100000
#define CHILDS 4

struct message_type1 {
	long type; // 1 - 4, process the data
	long offset, size;
};

struct message_type2 {
	long type; // 5, integrate the data
	int min;
	int ended;
};

void process_type1(int id) {
	key_t key;
	int msqid, *data, i, fd;
	struct message_type1 mt1;
	struct message_type2 mt2;
	long len1 = sizeof(struct message_type1) - sizeof(long);
	long len2 = sizeof(struct message_type2) - sizeof(long);
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok_type1");
		exit(-1);
	}
	
	if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror("msgget_type1");
        exit(-1);
    }
    
    while (msgrcv(msqid, &mt1, len1, id, IPC_NOWAIT) != -1) {
    	
		data = (int *) malloc(sizeof(int) * mt1.size);
		if (data == 0) {
			printf("no_memory__type1");
			exit(-1);
		}
		
		if ( (fd = open("data.dat", O_RDONLY)) < 0 ) {
			perror("open_type1");
			exit(-1);
		}
		lseek(fd, mt1.offset * sizeof(int), SEEK_SET);
		read(fd, data, mt1.size * sizeof(int));
		close(fd);
		
		mt2.type = 5;
		mt2.min = INT_MAX;
		mt2.ended = 0;
		for (i = 0; i < mt1.size; i++) {
			if (data[i] < mt2.min) {
				mt2.min = data[i];
			}
		}
		
		msgsnd(msqid, &mt2, len2, 0);
		
		free(data);
	}
	
	mt2.type = 5;
	mt2.min = -1;
	mt2.ended = 1;
	msgsnd(msqid, &mt2, len2, 0);
	
	exit(0);
}

void process_type2(int id) {
	key_t key;
	int msqid, min, i, ended;
	struct message_type2 mt2;
	long len2 = sizeof(struct message_type2) - sizeof(long);
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok_type1");
		exit(-1);
	}
	
	if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror("msgget_type2");
        exit(-1);
    }
    
    min = INT_MAX;
    ended = 0;
    while (ended < 4) {
    	msgrcv(msqid, &mt2, len2, id, 0);
    	if (mt2.ended) {
    		ended++;
    	} else if (min > mt2.min) {
			min = mt2.min;
		}
	}
	
	printf("MIN = %i\n", min);
	
	exit(0);
}

int main(int argc, char* argv[]) {
	key_t key;
	int msqid, i, pid;
	struct message_type1 mt1;
	long block_size, offset;
	long len1 = sizeof(struct message_type1) - sizeof(long);
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		exit(-1);
	}
    
    if ( (msqid = msgget(key, 0666 | IPC_CREAT)) == -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    i = 0;
    offset = 0;
    while (offset < SIZE) {
    	mt1.type = i + 1;
    	mt1.offset = offset;
    	if (offset + GRAIN <= SIZE) {
    		mt1.size = GRAIN;
    	} else {
    		mt1.size = SIZE - offset;
    	}
    	
    	msgsnd(msqid, &mt1, len1, 0);
    	
    	offset += GRAIN;
    	i = (i + 1) % CHILDS;
    }
    	
 		   	
    for (i = 0; i <= CHILDS; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			if (i != CHILDS) {
				process_type1(i + 1);
			} else {
				process_type2(i + 1);
			}
		} else {
			printf("PID = %i a process type %i has been created\n", pid, (i + 1));
		}
	}
    
    while (i > 0) {
		wait(NULL);
		i--;
	}
	
	msgctl(msqid, IPC_RMID, 0);
	
	return 0;
}
