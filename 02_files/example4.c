/**
* file: 	example4.c
* author:	Pedro Perez
* version:	26-02-2020
* description:
	In this file, you will find a code similar to the example3.c file; The
	difference is the creation of a dynamic buffer.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef unsigned char uchar;
typedef unsigned long ulong;

int main(int argc, char* argv[]) {
	int source, destination;

	if (argc != 3) {
		printf("usage: %s source destination\n", argv[0]);
		return -1;
	}

	if ( (source = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -2;
	}

	if ( (destination = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		return -3;
	}

	int bytes;
	uchar *buffer;
	ulong size = lseek(source, 0, SEEK_END);
	buffer = (uchar*) malloc(sizeof(uchar) * size);

	lseek(source, 0, SEEK_SET);
	read(source, buffer, sizeof(uchar) * size);
	write(destination, buffer, sizeof(uchar) * size);

	free(buffer);
	close(source);
	close(destination);
	return 0;
}
