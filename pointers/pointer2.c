#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000000

int main(int argc, char* argv[]) {
	int *arr, i;

	arr = (int*) malloc(SIZE * sizeof(int));
	for (i = 0; i < SIZE; i++) {
		arr[i] = i;
	}

	free(arr);
	return 0;
}
