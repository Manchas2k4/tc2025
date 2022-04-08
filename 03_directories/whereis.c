#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void find(char*, char*, char*);

int main(int argc, char* argv[]) {
	char dir_name[PATH_MAX + 1];
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "usage: %s filename [dir]\n", argv[0]);
		return -1;
	}

	if (argc == 2) {
		getcwd(dir_name, PATH_MAX);
	} else {
		strcpy(dir_name, argv[2]);
	}
	find(dir_name, argv[1], argv[0]);
	return 0;
}

void find(char *directory, char *file, char* program) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-2);
	}

	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {

			if (strcmp(direntry->d_name, file) == 0) {
				printf("find in %s\n", directory);
			}

			/*
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			stat(filename, &info);
			*/
			stat(direntry->d_name, &info);
			if (S_ISDIR(info.st_mode)) {
				find(filename, file, program);
			}
		}
	}

	closedir(dir);
}
