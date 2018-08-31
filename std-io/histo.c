#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE* file;
	int aux, total, num, i;
	int maximum;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		return -1;
	}
	
	file = fopen(argv[1], "r");
	if (!file) {
		perror(argv[1]);
		return -1;
	}

	//feof(file)
	maximum = 0;
	while (fscanf(file, "%i,%i", &aux, &total) != 0) {
		fprintf(stdout, "aux = %i - total = %i\n", aux, total);
		total *= total;
		for (i = 0; i < total; i++) {
			fscanf(file, ",%i", &num);
			//fprintf(stdout, "i = %i - num = %i\n", i, num);
			if (maximum < num) {
				maximum = num;
			}
		}
	}
	
	fprintf(stdout, "maximum = %i\n", maximum);
	
	fclose(file);
	
	return 0;
}
