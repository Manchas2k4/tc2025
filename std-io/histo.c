#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE* file;
	int total, num, counts[101];
	int maximo;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		return -1;
	}
	
	file = fopen(argv[1], "r");
	if (!file) {
		perror(argv[1]);
		return -1;
	}
	
	memset(counts, 0, sizeof(int) * 101);

	fscanf(file, "%i", &total);
	for (i = 0; i < total; i++) {
		fscanf(file, ",%i", &num);
		counts[num]++;
	}
	
	fclose(file);
	
	maximo = 0;
	for (int i = 1; i < 101; i++) {
		if (counts[i] > maximo) {
			maximo = counts[i];
		}
	}

	
	return 0;
}
