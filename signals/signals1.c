#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <signal.h>

void handler(int signal) {
    switch(signal) {
        case SIGUSR1 : printf("PID = %i, SIGNAL USR1\n", getpid()); break;
        case SIGUSR2 : printf("PID = %i, SIGNAL USR2\n", getpid()); break;
    }
}

int main(int argc, char* argv[]) {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    
    int i = 0;
    while (1) {
        printf("i = %i\n", i++);
        sleep(10);
    }  
    // kill(pid, SIGNAL);
    // raise(SIGNAL); - kill(getpid(), SIGNAL);
}
