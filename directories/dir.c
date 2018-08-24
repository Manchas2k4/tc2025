#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void get_info(char *name, char *directory, char* program) {
	int i;
	struct stat info;
	struct passwd *pw;
	struct group *gr;
	time_t rawtime;
	struct tm* timeinfo;
	char date[13];
	char permits[] = {'x', 'w', 'r'};
	char filename[PATH_MAX + NAME_MAX + 1];
	
	sprintf(filename, "%s/%s", directory, name);
	if (lstat(filename, &info) < 0) {
		perror(program);
		exit(-1);
	}
	
	 switch (info.st_mode & S_IFMT) {
		case S_IFDIR:  printf("d");	break;
		case S_IFLNK:  printf("l"); break;
		default		:  printf("-"); break;
	}
	
	for (i = 0; i < 9; i++) {
		if (info.st_mode & (0400 >> i)) {
			printf("%c", permits[(8 - i) % 3]);
		} else {
			printf("-");
		}
	}
	
	printf(" %2li", info.st_nlink);
	
	if ( (pw = getpwuid(info.st_uid)) == NULL ) {
		printf(" ????????");
	} else {
		printf(" %8s", pw->pw_name);
	}
	
	if ( (gr = getgrgid(info.st_gid)) == NULL ) {
		printf(" ????????");
	} else {
		printf(" %8s", gr->gr_name);
	}
	
	printf(" %7li", info.st_size);
	
	rawtime = info.st_mtime;
	timeinfo = localtime(&rawtime);
	strftime(date, 13, "%b %d %R", timeinfo);
	printf(" %s", date);
	
	printf(" %s\n", name);
}

void list(char *directory, int recursive, char* program) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	
	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	printf("DIRECTORY %s:\n", directory);
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			get_info(direntry->d_name, directory, program);
		}
	}
	
	if (recursive) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 &&
				strcmp(direntry->d_name, "..") != 0) {
				
				sprintf(filename, "%s/%s", directory, direntry->d_name);
				lstat(filename, &info);
				if (S_ISDIR(info.st_mode)) {
					list(filename, recursive, program);
				}
			}
		}
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;
	
	if (argc > 3) {
		printf("usage: %s [-r] [directory]\n", argv[0]);
		return -1;
	}
	
	getcwd(dir_name, NAME_MAX);
	directory = dir_name;
	recursive = 0;
	if (argc == 2) {
		if (strcmp(argv[1], "-r") == 0) {
			recursive = 1;
		} else {
			directory = argv[1];
		}
	} else if (argc == 3) {
		if (strcmp(argv[1], "-r") != 0) {
			printf("usage: %s [-r] [directory]\n", argv[0]);
			return -1;
		}
		recursive = 1;
		directory = argv[2];
	}
	list(directory, recursive, argv[0]);
	return 0;
}
