#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXLOC 50

int main(int argc, char* argv[]) {
  int i, fd, *buffer, loc;
  long size;

  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    return -1;
  }

  if ( (fd = open(argv[1], O_RDONLY)) < 0 ) {
    perror("open");
    return -2;
  }

  size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  loc = size / sizeof(int);
  buffer = (int*) malloc(sizeof(int) * loc);
  read(fd, buffer, sizeof(int) * loc);

  int max = (loc < MAXLOC)? loc : MAXLOC;
  for (i = 0; i < max; i++) {
    printf("%-12i", buffer[i]);
  }
  printf("\n");

  free(buffer);
  close(fd);

  return 0;
}
