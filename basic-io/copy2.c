#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SIZE 10000 //1048576

int main(int argc, char* argv[]) {
	int fd_in, fd_out, bytes;
	char buffer[SIZE];
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s source destination\n", argv[0]);
		return -1;
	}
	
	if ( (fd_in = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	while ( (bytes = read(fd_in, buffer, SIZE * sizeof(char))) != 0 ) {
		write(fd_out, buffer, bytes);
	}
	
	close(fd_in);
	close(fd_out);
	
	return 0;
}
