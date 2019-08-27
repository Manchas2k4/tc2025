#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SIZE 4096

typedef unsigned char uchar;

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
	uchar buffer[SIZE];
	while ( (bytes = read(source, buffer, sizeof(uchar) * SIZE)) != 0) {
		write(destination, buffer, bytes);
	}

	close(source);
	close(destination);
	return 0;
}
