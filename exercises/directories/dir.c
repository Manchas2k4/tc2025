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

void info(char* directory, char* name, char *program) {
	int i;
	char filename[NAME_MAX + 1];
	char permits[] = {'x', 'w', 'r'};
	char date[20];
	struct stat ss;
	struct passwd *pw;
	struct group *gr;
	time_t rawtime;
	struct tm *timeinfo;

	sprintf(filename, "%s/%s", directory, name);	
	if (lstat(filename, &ss) == -1) {
		perror(program);
		exit(-1);
	}
	
	if ((ss.st_mode & S_IFMT) == S_IFDIR) {
		printf("d");
	} else if (S_ISLNK(ss.st_mode)) {
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
		
	printf(" %2li", ss.st_nlink);
	
	if ( (pw = getpwuid(ss.st_uid)) == NULL ) {
		printf(" ????????");
	} else {
		printf(" %8s", pw->pw_name);
	}
	
	if ( (gr = getgrgid(ss.st_gid)) == NULL) {
		printf(" ????????");
	} else {
		printf(" %8s", gr->gr_name);
	}
	
	printf(" %7li", ss.st_size);
	
	rawtime = ss.st_mtime;
	timeinfo = localtime(&rawtime);
	strftime(date, 20, " %b %d %H:%M", timeinfo);
	printf(" %s", date);
	
	printf(" %s\n", name);
}

void list(char *directory, int recursive, char *program) {
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
		
		info(directory, dir_entry->d_name, program);
	}
	printf("\n");
	
	if (recursive) {
		rewinddir(dir);
		while ( (dir_entry = readdir(dir)) != NULL ) {
			if (strcmp(dir_entry->d_name, ".") == 0 ||
				strcmp(dir_entry->d_name, "..")  == 0) {
				continue;
			}
			sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
			stat(filename, &ss);
			if (S_ISDIR(ss.st_mode)) {
				list(filename, recursive, program);
			}
		}
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;
	
	if (argc > 3) {
		fprintf(stderr, "usage: %s [-r] [directory]\n", argv[0]);
		return -1;
	}
	
	strcpy(dir_name, ".");
	recursive = 0;
	directory = dir_name;
	if (argc == 2) {
		if (strcmp(argv[1], "-r") == 0) {
			recursive = 1;
		} else {
			directory = argv[1];
		}
	} else if (argc == 3) {
		if (strcmp(argv[1], "-r") == 0) {
			recursive = 1;
			directory = argv[2];
		} else {
			fprintf(stderr, "usage: %s [-r] [directory]\n", argv[0]);
			return -1;
		}
	}
	
	list(directory, recursive, argv[0]);
	return 0;
}
		
