#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <fcntl.h>

#define MAXBUFF 10000

int search(char* directory, char* name, char *word, char *program) {
	char filename[NAME_MAX + 1];
	char *buffer = (char*) malloc(MAXBUFF * sizeof(char));
	char *p;
	int fd, found;

	sprintf(filename, "%s/%s", directory, name);	
	if ( (fd = open(filename, O_RDONLY)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	found = 0;
	while ( read(fd, buffer, MAXBUFF * sizeof(char)) != 0 ) {
		if (strstr(buffer, word)) {
			found = 1;
			break;
		}
	}
	
	close(fd);
	
	return found;
}

void traverse(char *word, char *directory, char *program) {
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
		stat(filename, &ss);
		if (!S_ISDIR(ss.st_mode)) {
			if (search(directory, dir_entry->d_name, word, program)) {
				printf("%s ", dir_entry->d_name);
			}
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
		stat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			traverse(word, filename, program);
		}
	}
	
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory, * word;
	
	if (argc > 3) {
		fprintf(stderr, "usage: %s word [directory]\n", argv[0]);
		return -1;
	}
	
	strcpy(dir_name, ".");
	directory = dir_name;
	word = argv[1];
	
	if (argc == 3) {
		directory = argv[2];
	}
	
	traverse(word, directory, argv[0]);
	return 0;
}
