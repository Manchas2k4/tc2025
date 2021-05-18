#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 5

// limite [inf, sup)
typedef struct {
  int id, inf, sup;
} Limits;

void* task(void* param) {
  int i;
  Limits *limits;

  limits = (Limits*) param;
  for (i = limits->inf; i < limits->sup; i++) {
    printf("PID = %i, TID = % i, i = %i\n", getpid(), limits->id, i);
  }
  free(limits);
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_t tid[THREADS];
  Limits *limits;
  int i;

  for (i = 0; i < THREADS; i++) {
    limits = (Limits*) malloc(sizeof(Limits));
    limits->id = i;
    limits->inf = i * 100;
    limits->sup = (i + 1) * 100;
    pthread_create(&tid[i], NULL, task, (void*) limits);
  }

  for (i = 0; i < THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}
