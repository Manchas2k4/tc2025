#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>


jmp_buf env1, env2, env3;

void handler(int sig) {
	if (sig == SIGUSR1) {
    longjmp(env1, 1);
  } else if (sig == SIGUSR2) {
    longjmp(env2, 1);
  } else if (sig == SIGHUP) {
    longjmp(env3, 1);
  }
}

int main(int argc, char* argv[]) {
  int i;

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);
  signal(SIGHUP, handler);

  i = setjmp(env1);
  printf("Punto de retorno: 1\n");
  printf("Cuadrado de los primeros 10 numeros naturales\n");
  for (i = 1; i <= 10; i++) {
    printf("%i - %i\n", i, (i*i));
  }
  printf("\n");

  i = setjmp(env2);
  printf("Punto de retorno: 2\n");
  printf("Tabla del 2\n");
  for (i = 1; i <= 10; i++) {
    printf("2 * %i - %i\n", i, (2*i));
  }
  printf("\n");

  i = setjmp(env3);
  printf("Punto de retorno: 3\n");
  printf("Logaritmo de los primeros 10 numeros naturales\n");
  for (i = 1; i <= 10; i++) {
    printf("%i - %f\n", i, log(i));
  }
  printf("\n");

  while(1) {
    pause();
  }

  return 0;
}
