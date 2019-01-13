/**
	Copy a file (origin) to a new file (destination). The copy is made
	to redirect the standard input and output to the files; the reading
	is carried out as if it were made of a keyboard.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

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

	close(0);
	dup(fd_in);
	
	close(1);
	dup(fd_out);
	
	while (scanf("%c", &c) != EOF) {
		if ((c & 0x01) == 0) {
			printf("%c ", c);
		}
	}
	
	printf("proceso terminado\n");

	close(fd_in);
	close(fd_out);
	return 0;
}








