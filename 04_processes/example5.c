#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid;

    printf("pid = %i\n", getpid());
    if ( (pid = vfork()) < 0 ) {
      perror("fork");
      return -1;
    } else if (pid == 0) {
      printf("CHILD before pid = %i - ppid - %i\n", getpid(), getppid());
      execl("/bin/ls", "ls", "-l", "-a", (char*) 0);
      printf("CHILD after pid = %i - ppid - %i\n", getpid(), getppid());
      return 0;
    } else {
      wait(NULL);
    }
    printf("PARENT pid = %i\n", getpid());
    return 0;
}
