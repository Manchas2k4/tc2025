/**
* To compile: gcc pthread3.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

#define SIZE    1e9
#define THREADS 8

typedef struct {
  int start, end; // [start, end)
  int *arr;
} Block;

double sum(int *arr, int size) {
  double acum = 0;
  int i;

  for (i = 0; i < size; i++) {
    acum += arr[i];
  }
  return acum;
}

void* partial_sum(void *param) {
  double *acum;
  Block *block;
  int i;

  block = (Block *) param;
  acum = (double*) malloc(sizeof(double));
  (*acum) = 0;
  for (i = block->start; i < block->end; i++) {
    (*acum) += block->arr[i];
  }
  return ( (void **) acum );
}

int main(int argc, char* argv[]) {
  int *arr, block_size, i;
  double result, seq, concur, *acum;
  Block blocks[THREADS];
  pthread_t tids[THREADS];

  arr = (int *) malloc(sizeof(int) * SIZE);
  fill_array(arr, SIZE);
  display_array("arr:", arr);

  start_timer();
  result = sum(arr, SIZE);
  seq = stop_timer();
  printf("sum = %.0lf - time = %.4lf\n", result, seq);

  block_size = SIZE / THREADS;
  for (i = 0; i < THREADS; i++) {
    blocks[i].start = i * block_size;
    blocks[i].end = MIN((i + 1) * block_size, SIZE);
    blocks[i].arr = arr;
  }

  start_timer();
  result = 0;
  for (i = 0; i < THREADS; i++) {
    pthread_create(&tids[i], NULL,
      partial_sum, (void *) &blocks[i]);
  }
  for (i = 0; i < THREADS; i++) {
		pthread_join(tids[i], (void *) &acum);
    result += (*acum);
    free(acum);
	}
  concur = stop_timer();
  printf("sum = %.0lf - time = %.4lf\n", result, concur);

  printf("speedup = %.6lf\n", (seq / concur));
}
