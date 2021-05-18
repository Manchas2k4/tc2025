#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 5
#define LIMIT   5

void* task(void* param) {
  int i, id;

  id = *((int*) param);
  for (i = 0; i < LIMIT; i++) {
    printf("PID = %i, TID = % i, i = %i\n", getpid(), id, i);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_t tid[THREADS];
  int i;

  for (i = 0; i < THREADS; i++) {
    pthread_create(&tid[i], NULL, task, (void*) &i);
  }

  for (i = 0; i < THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}
