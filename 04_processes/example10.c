#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid;
    char* arr[] = {"ls", "-l", "-a", (char*) 0};
    char* env[] = {"PATH=$PATH$:.", (char*) 0};

    printf("pid = %i\n", getpid());
    if ( (pid = vfork()) < 0 ) {
      perror("fork");
      return -1;
    } else if (pid == 0) {
      printf("before pid = %i - ppid - %i\n", getpid(), getppid());
      execve("hello", arr, env);
      //execv("/bin/ls", (argv + 1));
      printf("after pid = %i - ppid - %i\n", getpid(), getppid());
      return 0;
    } else {
      wait(NULL);
    }
    printf("pid = %i\n", getpid());
    return 0;
}
