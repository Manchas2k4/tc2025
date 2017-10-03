#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 	1000
#define WIDTH 	68

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

int max(int a, int b) {
	return ( (a > b)? a : b );
}

int convert(int val, int width, int max) {
	return (int) ((val * width) / max);
}

void display_histogram() {
	int i, m, j, val;
	
	m = 0;
	for (i = 1; i < SIZE; i++) {
		m = max(m, values[i]);
	}
	
	for (i = 1; i < SIZE; i++) {
		values[i] = convert(values[i], WIDTH, m);
	}
	
	for (i = 1; i < SIZE; i++) {
		if (values[i] != 0) {
			fprintf(stdout, "[%2i] : ", i);
			for (j = 0; j < values[i]; j++) {
				fprintf(stdout, "*");
			}
			fprintf(stdout, "\n");
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
