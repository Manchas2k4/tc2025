#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void execute(char *program, char *order1, char *order2, char *order3) {
    int p1p2[2], p2p3[2];
    int pid;
    
    // THE FIRST
    if (pipe(p1p2) < 0) {
        perror(program);
        exit(-1);
    }
    
    if ( (pid = fork()) < 0 ) {
        perror(program);
        exit(-1);
    } if (pid == 0) {
        // PADRE
        if (pipe(p2p3) < 0) {
            perror(program);
            exit(-1);
        }
        
        if ( (pid = fork()) < 0 ) {
            perror(program);
            exit(-1);
        } else if (pid == 0) {
            // CHILD
            close(1);
            dup(p2p3[1]);
            close(p2p3[0]);
            close(p2p3[1]);
            execlp(order1, order1, (char*) 0);
        } else {
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);
            close(p2p3[1]);
            
            close(1);
            dup(p1p2[1]);
            close(p1p2[0]);
            close(p1p2[1]);
            
            execlp(order2, order2, (char*) 0);
        }
    } else {
        // ABUELO
        close(0);
        dup(p1p2[0]);
        close(p1p2[0]);
        close(p1p2[1]);
        
        execlp(order3, order3, (char*) 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("usage: %s order1 order2 order3\n", argv[0]);
        return -1;
    }
    
    execute(argv[0], argv[1], argv[2], argv[3]);
    return 0;
}
