/**
	Copy a file (origin) to a new file (destination). The 
	copy is made to redirect the standard input and output 
	to the files; the reading is carried out as if it were
	made of a keyboard.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

#define OFFSET 10240

int main(int argc, char* argv[]) {
	int fd;
	
	if (argc != 2) {
		printf("forma de uso: %s archivo\n", argv[0]);
		return -1;
	}

	if ( (fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	char c = 'a';
	lseek(fd, OFFSET, SEEK_SET);
	write(fd, &c, sizeof(char));
	
	close(fd);
	return 0;
}








