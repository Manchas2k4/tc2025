#include "header.h"

int main(int argc, char* argv[]) {
    struct message msg;
    int msqid;
    int length = sizeof(struct message) - sizeof(long);
    long msg_type;
    key_t key;
    
    if (argc != 2) {
        printf("usage: %s msg_type\n", argv[0]);
        return -1;
    }
 
    msg_type = atol(argv[1]);
    if (msg_type < 0 || msg_type > 4) {
        printf("%s: the msg_type must be between 0 and 4.\n", argv[0]);
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
    
    while (msgrcv(msqid, &msg, length, msg_type, IPC_NOWAIT) != -1) {
		printf("MESSAGE READ: msg_type = %li process = %li value = %i\n", msg.type, msg.process, msg.value);
	}
	
	return 0;
}
