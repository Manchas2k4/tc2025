#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void get_info(char *name, char *directory, char *program);
void list(char* dir_name, int recursive, char* program);

int main(int argc, char* argv[]) {
	char dir_name[PATH_MAX + 1];
	char *directory;
	int recursive;

	if (argc > 3) {
		fprintf(stderr, "usage: %s [-r] [directory]\n", argv[0]);
		return -1;
	}

	getcwd(dir_name, PATH_MAX);
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
			fprintf(stderr, "usage: %s [-r] [directory]\n", argv[0]);
			return -1;
		}
		recursive = 1;
		directory = argv[2];
	}

	list(directory, recursive, argv[0]);
	return 0;
}

void list(char *dir_name, int recursive, char *program) {
	char path[PATH_MAX + NAME_MAX + 1];
	DIR *dir;
	struct dirent *direntry;
	struct stat info;

	if ( (dir = opendir(dir_name)) == NULL ) {
		perror(program);
		exit(-2);
	}

	printf("%s:\n", dir_name);
	while( (direntry = readdir(dir)) != NULL) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {

			get_info(direntry->d_name, dir_name, program);
		}
	}
	if (recursive) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 &&
			    strcmp(direntry->d_name, "..") != 0) {

				sprintf(path, "%s/%s", dir_name, direntry->d_name);
				lstat(path, &info);
				if ( (info.st_mode & S_IFMT) == S_IFDIR) {
					list(path, recursive, program);
				}
			}
		}
	}
	closedir(dir);
	printf("\n");
}

void get_info(char *name, char *directory, char *program) {
	int i;
	struct stat info;
	struct passwd *pw;
	struct group *gr;
	time_t raw_time;
	struct tm *timeinfo;
	char date[13];
	char permits[] = {'x', 'w', 'r'};
	char filename[PATH_MAX + NAME_MAX + 1];

	sprintf(filename, "%s/%s", directory, name);
	lstat(filename, &info);

	switch(info.st_mode & S_IFMT) {
		case S_IFDIR : printf("d"); break;
		case S_IFLNK : printf("l"); break;
		default		 : printf("-"); break;
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

	printf(" %7li", info.st_size);

	raw_time = info.st_mtime;
	timeinfo = localtime( &raw_time );
	strftime(date, 13, "%b %d %R", timeinfo);
	printf(" %s", date);

	printf(" %s\n", name);
}
