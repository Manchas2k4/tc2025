/**
* file: 	example1.c
* author:	Pedro Perez
* version:	26-02-2020
* description: 
	In this file, you will find how data is received from the command line. You
	will also see how to validate that a program receives the correct number of 
	parameters.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int i;

	if (argc != 5) {
		printf("usage: %s arg1 arg2 arg3 arg4\n", argv[0]);
		return -1;
	}

	for (i = 0; i < argc; i++) {
		printf("argv[%i] = %s\n", i, argv[i]);
	}
	return 0;
}
