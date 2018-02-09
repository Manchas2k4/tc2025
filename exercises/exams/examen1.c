#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

void min_max(char* filename, char* program) {
	int fd, min, max, number;
	
	min = INT_MAX;
	max = INT_MIN;
	
	if ( (fd = open(filename, O_RDONLY)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	while ( read(fd, &number, sizeof(int)) != 0 ) {
		if (min > number) {
			min = number;
		} else if (max < number) {
			max = number;
		}
	}
	
	printf("file %s - min = %i - max = %i\n", filename, min, max);
	close(fd);
}

void traverse(char *directory, char *program) {
	char filename[NAME_MAX + 1];
	DIR *dir;
	struct dirent *dir_entry;
	struct stat ss;
	
	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	printf("%s:\n", directory);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		lstat(filename, &ss);
		if (S_ISREG(ss.st_mode)) {
			min_max(filename, program);
		}
	}
	printf("\n");
	
	rewinddir(dir);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		lstat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			traverse(filename, program);
		}
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s directory\n", argv[0]);
		return -1;
	}
	
	traverse(argv[1], argv[0]);
	return 0;
}
