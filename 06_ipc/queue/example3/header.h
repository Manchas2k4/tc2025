#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

typedef struct {
	long type; // 1 - 4, process the data
	long offset, size;
} Type1;

typedef struct {
	long type; // 5, integrate the data
	int min;
	int ended;
} Type2;

#endif
