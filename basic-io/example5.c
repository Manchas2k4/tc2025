#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned int uint;

typedef struct link {
	int value;
	uint next;
} Link;

typedef struct list {
	uint head, free, size;
	Link *data;
} List;

List l;
fd = open("list", O_WRONLY);
write(fd, &l, sizeof(List) * (sizeof(Link) * l.size));

















