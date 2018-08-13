#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 10000000

int main(int argc, char* argv[]) {
	int *arr, i;

	arr = (int*) malloc(SIZE * sizeof(int));
	for (i = 0; i < SIZE; i++) {
		arr[i] = i;
	}
	
	for (i = 0; i < 100; i++) {
		printf("%i ", arr[i]);
	}
	printf("....\n");

	free(arr);
	return 0;
}
