#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OFFSET 17528

int main(int argc, char* argv[]) {
	int fd;
	
	if (argc != 2) {
		printf("usage: %s filename\n", argv[0]);
		return -1;
	}
	
	if ( (fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	char c = 'a';
	write(fd, &c, sizeof(char));
	
	c = 'j';
	lseek(fd, OFFSET, SEEK_SET);
	write(fd, &c, sizeof(char));
	
	close(fd);
	
	return 0;
}

