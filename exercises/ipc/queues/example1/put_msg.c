#include "header.h"

int main(int argc, char* argv[]) {
    struct message msg;
    int i, msqid, num_msg;
    int length = sizeof(struct message) - sizeof(long);
    long msg_type;
    key_t key;
    
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
        return -1;
    }
 
    if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    srand(time(0));
	num_msg = (rand() % 30) + 1;
	for (i = 0; i < num_msg; i++) {
		msg.type = (rand() % 4) + 1;
		msg.process = getpid();
		msg.value = (rand() % 100) + 1;
		msgsnd(msqid, &msg, length, 0);
		printf("MESSAGE POSTED: msg_type = %li process = %li value = %i\n", msg.type, msg.process, msg.value);
	}
	
	return 0;
}
