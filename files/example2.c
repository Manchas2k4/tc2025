/**
	Copy a file (origin) to a new file (destination). The 
	copy is made by reading one block of bytes at a time.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 2028

int main(int argc, char* argv[]) {
	int fd_in, fd_out;
	
	if (argc != 3) {
		printf("usage: %s origin destination\n", argv[0]);
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
	
	char buffer[SIZE];
	ssize_t nbytes;
	while ( (nbytes = read(fd_in, buffer, SIZE)) != 0 ) {
		write(fd_out, buffer, nbytes);
	}	
	
	close(fd_in);
	close(fd_out);
	return 0;
}
