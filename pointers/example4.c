/**
	This program uses function pointers to make the 
	comparison when ordering an array using Bubble Sort.
	
	@author Pedro Perez
	@version 2.0 13/01/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void swap(int *a, int *b) {
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int greater_than(int a, int b) {
	return (a > b);
}

int less_than(int a, int b) {
	return (a < b);
}

void bubble_sort(int arr[], int size, int (*fn) (int, int) ) {
	int i, j;
	
	for (i = size - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if ( fn(arr[j], arr[j + 1]) ) {
				swap((arr + j), (arr + j + 1));
			}
		}
	}
}

void display(int arr[], int size) {
	int i;
	
	printf("[%i", arr[0]);
	for (i = 1; i < size; i++) {
		printf(",%i", arr[i]);
	}
	printf("]\n");
}

int main(int argc, char* argv[]) {
	int a[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	
	printf("initial array: ");
	display(a, 10);
	printf("ascend array: ");
	bubble_sort(a, 10, greater_than);
	display(a, 10);
	printf("\n\n");
	
	printf("initial array: ");
	display(a, 10);
	printf("descend array: ");
	bubble_sort(a, 10, less_than);
	display(a, 10);
	
	return 0;
}
