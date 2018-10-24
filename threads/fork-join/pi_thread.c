/**
* To compile: gcc pi_thread.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils/cheader.h"

#define NUM_RECTS	1e8
#define MAXTHREADS 	4

typedef struct parameters {
	double width;
	int start, end;
} Parameters;


void* task(void* param) {
	int i;
	double mid, height;
	double* sum = (double*) malloc(sizeof(double));
	Parameters* p = (Parameters*) param;
	
	(*sum) = 0;
	for (i = p->start; i < p->end; i++) {
		mid = (i + 0.5) * p->width;
		height = 4.0 / (1.0 + (mid * mid));
		(*sum) += height;
	}
	pthread_exit((void*) sum);
}

int main(int arg, char* argv[]) {
	pthread_t pthread_id[MAXTHREADS];
	Parameters params[MAXTHREADS];
	void* sum[MAXTHREADS];
	
	int i, j, block;
	double width, area, ms, aux;
	
	width = 1.0 / (double) NUM_RECTS;
	block = NUM_RECTS / MAXTHREADS;


	for (i = 0; i < MAXTHREADS; i++) {
		params[i].width = width;
		params[i].start = i * block;
		if (i != MAXTHREADS - 1) {
			params[i].end = (i + 1) * block;
		} else {
			params[i].end = NUM_RECTS;
		}
	}

	for (j = 1; j < N; j++) {
		start_timer();	
		for (i = 0; i < MAXTHREADS; i++) {
			pthread_create(&pthread_id[i], NULL, task, (void*) &params[i]);
		}
	
		for (i = 0; i < MAXTHREADS; i++) {
			pthread_join(pthread_id[i], &sum[i]);
		}
		ms += stop_timer();
	}
	
	aux = 0;
	for (i = 0; i < MAXTHREADS; i++) {
		aux += *((double*) sum[i]);
	}
	area = width * aux;
	printf("PI = %.15lf\n", area);
	printf("avg time = %.5lf ms\n", (ms / N));
	return 0;
	return 0;
}
