#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void intercambia(int *a, int *b) {
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int es_mayor(int a, int b) {
	return (a > b);
}

int es_menor(int a, int b) {
	return (a < b);
}

int aleatorio(int a, int b) {
	if (rand() % 2 == 0) {
		return 1;
	} else {
		return 0;
	}
}

// void quick_sort(void *arr, int num_loc, int data_size, int (*fn) (int, int) ) {
void bubble_sort(int arr[], int size, int (*fn) (int, int) ) {
	int i, j;
	
	for (i = size - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if ( fn(arr[j], arr[j + 1]) ) {
				intercambia((arr + j), (arr + j + 1));
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
	
	srand(time(0));
	
	display(a, 10);
	bubble_sort(a, 10, es_mayor);
	display(a, 10);
	bubble_sort(a, 10, es_menor);
	display(a, 10);
	bubble_sort(a, 10, aleatorio);
	display(a, 10);
	
	return 0;
}
