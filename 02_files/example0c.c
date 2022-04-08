#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
  int fd;

  fd = open("patito.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
  printf("fd %d\n", fd);
  return 0;
}
