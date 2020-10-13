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

#define PCT_RED 0.299
#define PCT_GREEN 0.587
#define PCT_BLUE 0.114

enum Chanel {RED, GREEN, BLUE};

typedef unsigned char uchar;
typedef unsigned int uint;

int main(int argc, char* argv[]) {
  int img_file;
  uint width, height, i;
  uchar *img_data;

  if (argc != 2) {
      fprintf(stderr, "usage: %s img_file\n", argv[0]);
      return -1;
  }

  if ( (img_file = open(argv[1], O_RDWR)) < 0 ) {
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

  lseek(img_file, 28, SEEK_CUR);
  read(img_file, img_data, sizeof(uchar) * width * height * 3);

  uchar avg = 0;
  for (i = 0; i < (width * height); i++) {
    avg = (uchar) ((img_data[(i * 3) + RED] +
                    img_data[(i * 3) + GREEN] +
                    img_data[(i * 3) + BLUE]) / 3.0);
    img_data[(i * 3) + RED] = avg;
    img_data[(i * 3) + GREEN] = avg;
    img_data[(i * 3) + BLUE] = avg;
  }

  lseek(img_file, 54, SEEK_SET);
  write(img_file, img_data, sizeof(uchar) * width * height * 3);

  free(img_data);
  close(img_file);
  return 0;
}
