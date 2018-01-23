#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	int fd;
	unsigned int width, height;
	unsigned short int  colordepth;
	unsigned long length_array;
	char *img;
		
	if (argc != 2) {
		fprintf(stderr, "usage: %s image_file\n", argv[0]);
		return -1;
	}
	
	if ( (fd = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[1]);
		return -1;
	}
	
	lseek(fd, 18, SEEK_SET);
	read(fd, &width, sizeof(unsigned int));
	read(fd, &height, sizeof(unsigned int));
	
	lseek(fd, 2, SEEK_CUR);
	read(fd, &colordepth, sizeof(unsigned short int));
	
	if (colordepth != 24) {
		fprintf(stderr, "%s: the image must be RGB\n", argv[0]);
		close(fd);
		return -1;
	}
	
	lseek(fd, 54, SEEK_SET);
	
	length_array = (width * height) * 3 * sizeof(char);
	img = (char*) malloc(length_array);
	read(fd, img, length_array);
	
	int i;
	for (i = 1; img[i] != '\0'; i += 3) {
		printf("%c", img[i]);
	}
	
	close(fd);
			
	return 0;
}
