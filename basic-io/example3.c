#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	
	off_t length;
	length = lseek(fd_in, 0, SEEK_END);
	lseek(fd_in, 0, SEEK_SET);
	
	char *buffer;
	buffer = (char*) malloc(length * sizeof(char));
	
	ssize_t nbytes;
	while ( (nbytes = read(fd_in, buffer, length)) != 0 ) {
		write(fd_out, buffer, nbytes);
	}	
	
	free(buffer);
	
	close(fd_in);
	close(fd_out);
	return 0;
}
