/**
* file: 	example5.c
* author:	Pedro Perez
* version:	26-02-2020
* description: 
	In this file, you will find the code that extracts the information hidden
	with the previous example.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define END 240

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

int main(int argc, char* argv[]) {
  int txt_file, img_file;
  uint width, height, i;
  ulong txt_size;
  uchar *txt_data, *img_data;

  if (argc != 2) {
      fprintf(stderr, "usage: %s img_file\n", argv[0]);
      return -1;
  }

  if ( (img_file = open(argv[1], O_RDONLY)) < 0 ) {
    perror(argv[0]);
    return -2;
  }

  lseek(img_file, 18, SEEK_SET);
  read(img_file, &width, sizeof(uint));
  read(img_file, &height, sizeof(uint));
  
  img_data = (uchar*) malloc(sizeof(uchar) * width * height * 3);
  if (img_data == NULL) {
  	printf("%s: No memory space for IMG\n", argv[0]);
  	return -3;
  }
  lseek(img_file, 54, SEEK_SET);
  read(img_file, img_data, sizeof(uchar) * width * height * 3);
  
  i = 0;
  while (img_data[i] != END) {
  	printf("%c", img_data[i]);
  	i = i + 3;
  }
  free(img_data);
  close(img_file);
  return 0;
}
