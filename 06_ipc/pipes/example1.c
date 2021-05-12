#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


int main(int argc, char* argv[]) {
  int pid, p12[2];

  if (argc !=3) {
    printf("usage: %s command1 command2\n", argv[0]);
    return -1;
  }

  pipe(p12);
  if ( (pid = fork()) < 0 ) {
    perror("fork");
    return -1;
  } else if (pid == 0) {
    close(1);
    dup(p12[1]);

    close(p12[0]);
    close(p12[1]);
    execlp(argv[1], argv[1], (char*) 0);
  } else {
    close(0);
    dup(p12[0]);

    close(p12[0]);
    close(p12[1]);
    execlp(argv[2], argv[2], (char*) 0);
  }
  return 0;
}
