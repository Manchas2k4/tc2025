#include "header.h"

int main(int argc, char* argv[]) {
    int msqid;
    key_t key;
    
    if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    if ( (msqid = msgget(key, 0666 | IPC_CREAT)) == -1 ) {
        perror(argv[0]);
        return -1;
    }
    printf("The queue %i has been created.\n", msqid);
    return 0;
}
