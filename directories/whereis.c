#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void list(char *directory, char *word, char* program) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	
	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {
			
			//printf("%s - %s\n", direntry->d_name, word);
			if (strcmp(direntry->d_name, word) == 0) {
				printf("%s ", directory);
			}
			
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			lstat(filename, &info);
			if (S_ISDIR(info.st_mode)) {
				//printf("%s is dir\n", filename);
				list(filename, word, program);
			} 
		}
	}
}

int main(int argc, char* argv[]) {
	char dir_name[PATH_MAX + 1];
	
	if (argc != 2) {
		printf("usage: %s word\n", argv[0]);
		return -1;
	}
	
	getcwd(dir_name, PATH_MAX);
	printf("%s : ", argv[1]);
	list(dir_name, argv[1], argv[0]);	
	printf("\n");
}
