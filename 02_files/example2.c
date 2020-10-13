/**
* file: 	example2.c
* author:	Pedro Perez
* version:	26-02-2020
* description: 
	In this file, you will find the code that allows you to create a "hole" in
	an ordinary file. The program creates an empty file, and due to the 
	logical-physical handling of UNIX, you can advance to byte 5,000 to write a
	single character.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
  int fd;
  char buffer;

  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    return -1;
  }

  if ( (fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
    perror("open");
    return -2;
  }

  buffer = 'a';
  lseek(fd, 5000, SEEK_SET);
  write(fd, &buffer, sizeof(char));

  close(fd);

  return 0;
}
