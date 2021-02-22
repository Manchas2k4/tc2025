#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  int i, fd, amount, *buffer;
  char *alt_buffer;

  if (argc != 3) {
    printf("usage: %s filename amount\n", argv[0]);
    return -1;
  }

  if ( (fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
    perror("open");
    return -2;
  }

  amount = atoi(argv[2]);
  if (amount < 1) {
    printf("%s: amount must be greater than 0\n", argv[0]);
    return -3;
  }

  buffer = (int*) malloc(sizeof(int) * amount);

  srand(12345);
  for (i = 0; i < amount; i++) {
    buffer[i] = rand(); //(rand() % 100) + 1;
  }

  write(fd, buffer, sizeof(int) * amount);

  free(buffer);
  close(fd);

  return 0;
}
