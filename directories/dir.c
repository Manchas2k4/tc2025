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

void info(char* filename, char *program) {
	int i;
	char permits[] = {'x', 'w', 'r'};
	char date[13];
	struct stat ss;
	struct passwd *pw;
	struct group *gr;
	time_t rawtime;
	struct tm *timeinfo;
	
	if (lstat(filename, &ss) == -1) {
		perror(program);
		exit(-1);
	}
	
	if ((ss.st_mode & S_IFMT) == S_IFDIR) {
		printf("d");
	else if (S_ISLNK(ss.st_mode)) {
		printf("l");
	} else {
		printf("-");
	}
	
	for (i = 0; i < 9; i++) {
		if (ss.st_mode & (0400 >> i)) {
			printf("%c", permits[(8 - i) % 3]);
		} else {
			printf("-");
		}
	}
		
	
	
	

void list(char *directory, char *program) {
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
		info(filename, program);
	}
	printf("\n");
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;
	
	if (argc > 2) {
		fprintf(stderr, "usage: %s [directory]\n", argv[0]);
		return -1;
	}
	
	getcwd(dir_name, NAME_MAX);
	directory = dir_name;
	if (argc == 2) {
		directory = argv[1];
	} 
	
	list(directory, argv[0]);
	return 0;
}
		
