#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 5
#define LIMIT   5

void* task(void *param) {
  int i;

  for (i = 0; i < LIMIT; i++) {
    printf("PID = %i, TID = % li, i = %i\n", getpid(),
          pthread_self(), i);
    sleep(1);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_t tid;

  pthread_create(&tid, NULL, task, NULL);

  pthread_join(tid, NULL);

  return 0;
}
