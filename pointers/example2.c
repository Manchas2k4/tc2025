/**
	This program attempts to declare and initialize a 
	static array of 100,000,000 whole elements. 
	Apparently ... it can not do it.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//SIZE = 100,000,000
#define SIZE 100000000

int main(int argc, char* argv[]) {
	int i, arr[SIZE];

	for (i = 0; i < SIZE; i++) {
		arr[i] = i;
	}
	
	for (i = 0; i < 100; i++) {
		printf("%i ", arr[i]);
	}
	printf("....\n");

	return 0;
}
