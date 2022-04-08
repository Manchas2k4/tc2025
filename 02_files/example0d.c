#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
  int i;

  printf("argc = %i\n", argc);
  for (i = 0; i < argc; i++) {
    printf("%s ", argv[i]);
  }

  return 0;
}
