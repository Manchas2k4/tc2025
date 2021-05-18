/*************************************************************
* File: problem2.c
* Author: Pedro Perez
* Description: This file contains the implementation of
*              multiplication of a matrix by a vector using
*              pthreads.
*
* To compile: gcc problem2.c -pthread
*
* Copyright (c) 2021 by Tecnologico de Monterrey.
* All Rights Reserved. May be reproduced for any non-commercial
* purpose.
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

#define RENS 30000
#define COLS 30000
#define THREADS 8

/*************************************************************
 * Sequential implementation
 *************************************************************/
void matrix_vector(int *m, int *b, int *c) {
  int i, j, acum;

  for (i = 0; i < RENS; i++) {
    acum = 0;
    for (j = 0; j < COLS; j++) {
      acum += (m[(i * COLS) + j] * b[i]);
    }
    c[i] = acum;
  }
}

/*************************************************************
 * Concurrent implementation
 *************************************************************/
 typedef struct {
   int start, end; // [start, end)
   int *m, *b, *c;
 } Block;

void* partial_mult(void* param) {
  Block *block;
  int acum, i, j;

  block = (Block *) param;
  for (i = block->start; i < block->end; i++) {
    acum = 0;
    for (int j = 0; j < COLS; j++) {
      acum += (block->m[(i * COLS) + j] * block->b[i]);
    }
    block->c[i] = acum;
  }
  return ( (void*) 0 );
}

/*************************************************************
 * Main
 *************************************************************/
int main(int argc, char* argv[]) {
  int block_size, i, j;
  double result, seq, concur;
  Block blocks[THREADS];
  pthread_t tids[THREADS];
  int *m, *b, *c;

  m = (int*) malloc(sizeof(int) * RENS* COLS);
	b = (int*) malloc(sizeof(int) * RENS);
	c = (int*) malloc(sizeof(int) * RENS);

  for (i = 0; i < RENS; i++) {
		for (j = 0; j < COLS; j++) {
			m[(i * COLS) + j] = (j + 1);
		}
		b[i] = 1;
	}

  printf("Running sequential code..\n");
  seq = 0;
  for (i = 0; i < N; i++) {
    start_timer();
    matrix_vector(m, b, c);
    seq += stop_timer();
  }
  display_array("c", c);
  printf("time = %.4lf\n", (seq / N));

  block_size = RENS / THREADS;
  for (i = 0; i < THREADS; i++) {
    blocks[i].m = m;
    blocks[i].b = b;
    blocks[i].c = c;
    blocks[i].start = i * block_size;
    if (i != (THREADS - 1)) {
      blocks[i].end = (i + 1) * block_size;
    } else {
      blocks[i].end = RENS;
    }
  }

  printf("Running concurrent code..\n");
  concur = 0;
  for (j = 0; j < N; j++) {
    start_timer();
    result = 0;
    for (i = 0; i < THREADS; i++) {
      pthread_create(&tids[i], NULL,
        partial_mult, (void*) &blocks[i]);
    }
    for (i = 0; i < THREADS; i++) {
  		pthread_join(tids[i], NULL);
  	}
    concur += stop_timer();
  }
  display_array("c", c);
  printf("time = %.4lf\n", (concur / N));

  printf("speedup = %.6lf\n", (seq / concur));

  free(m); free(b); free(c);
}
