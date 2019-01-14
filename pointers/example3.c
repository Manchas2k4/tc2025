/**
	Like the previous example (example2.c), this program
	attempts to declare and initialize an array of 
	100,000,000 whole elements. However, this time dynamic
	memory is used.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//SIZE = 100,000,000
#define SIZE 100000000

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
