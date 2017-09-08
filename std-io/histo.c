#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1000

int values[SIZE];

void read_values(char* filename, char* program) {
	FILE *file;
	int time, intersections, size, i;
	float num;
	
	file = fopen(filename, "r");
	if (file == NULL) {
		perror(program);
		exit(-1);
	}
	
	memset(values, 0, SIZE * sizeof(int));
	
	while ( fscanf(file, "%i,%i", &time, &intersections) != EOF ) {
		size = intersections * intersections;
		for (i = 0; i < size; i++) {
			fscanf(file, ",%f", &num);
			values[ ((int) num) ]++;
		}
	}
	
	fclose(file);
}

void display_histogram() {
	int i;
	
	for (i = 1; i < SIZE; i++) {
		if (values[i] != 0) {
			fprintf(stdout, "[%i] : %i\n", i, values[i]);
		}
	} 
}

int main (int argc, char* argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return -1;
	}
	
	read_values(argv[1], argv[0]);
	display_histogram();
	
	return 0;
}
