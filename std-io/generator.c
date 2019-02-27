#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SIZE 	1000000
#define MAX		100

int main(int argc, char* argv[]) {
	FILE* file;
	int total, num, i;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		return -1;
	}
	
	file = fopen(argv[1], "w");
	if (!file) {
		perror(argv[1]);
		return -1;
	}

	srand(time(0));
	fprintf(file, "%i", SIZE);
	for (i = 0; i < SIZE; i++) {
		num = rand() % MAX + 1;
		fprintf(file, ",%i", num); 
	}
	
	fclose(file);
	
	return 0;
}
