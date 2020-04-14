#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env1, env2, env3;

void handler_sigusr1(int sig) {
	longjmp(env1, 1);
}

void handler_sigusr2(int sig) {
	longjmp(env2, 1);
}

void handler_sighup(int sig) {
	longjmp(env3, 1);
}

void fun1() {
  int i;

  setjmp(env1);
  printf("Punto de retorno: 1\n");
  printf("Cuadrado de los primeros 10 numeros naturales\n");
  for (i = 1; i <= 10; i++) {
    printf("%i - %i\n", i, (i*i));
  }
  printf("\n");
}

void fun2() {
  int i;

  setjmp(env2);
  printf("Punto de retorno: 2\n");
  printf("Tabla del 2\n");
  for (i = 1; i <= 10; i++) {
    printf("2 * %i - %i\n", i, (2*i));
  }
  printf("\n");
}

void fun3() {
  int i;

  setjmp(env3);
  printf("Punto de retorno: 3\n");
  printf("Logaritmo de los primeros 10 numeros naturales\n");
  for (i = 1; i <= 10; i++) {
    printf("%i - %f\n", i, log(i));
  }
  printf("\n");
}


int main(int argc, char* argv[]) {
  int i;

  signal(SIGUSR1, handler_sigusr1);
  signal(SIGUSR2, handler_sigusr2);
  signal(SIGHUP, handler_sighup);

  fun1();
  fun2();
  fun3();

  while(1) {
    pause();
  }

  return 0;
}
