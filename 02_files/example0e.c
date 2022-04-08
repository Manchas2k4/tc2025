#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
  int i, fd_in, fd_out;

  if (argc != 3) {
    printf("usage: %s input_file output_file\n", argv[0]);
    //fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
    return -1;
  }

  if ( (fd_in = open(argv[1], O_RDONLY)) < 0 ) {
    perror("open input_file");
    return -2;
  }

  if ( (fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0 ) {
    perror("open output_file");
    return -3;
  }

  
  return 0;
}
