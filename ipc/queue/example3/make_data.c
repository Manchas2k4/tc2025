#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define SIZE 10000000

int main(int argc, char* argv[]) {
	int fd, i, num, *data;
	
	srand( time(0) );
	data = (int*) malloc(SIZE * sizeof(int));
	for (i = 0; i < SIZE; i++) {
		data[i] = abs( rand() );
		
	}
	
	if ( (fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	write(fd, data, SIZE * sizeof(int));
	close(fd);
	
	
	free(data);
	return 0;
}
