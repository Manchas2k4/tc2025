#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct record {
  int a;
  double b;
  char c[100];
}; // sizeof(struct record) => 112

union option {
  int a;
  double b;
  char c[100];
}; // sizeof(struct record) => 100

int main(int argc, char* argv[]) {
  struct record var1;
  union option var2;

  var1.a = 10;
  var1.b = 20;
  strcpy(var1.c, "hola");
  printf("%i, %lf, %s\n\n", var1.a, var1.b, var1.c);

  strcpy(var2.c, "hola");
  //var2.b = 20;
  //var2.a = 10;
  printf("%i, %lf, %s\n\n", var2.a, var2.b, var2.c);

  int a[10];
  double b[20];
  char c[1000];

  memset(a, -1, sizeof(int) * 10);
  memset(b, 0, sizeof(double) * 20);
  memset(c, 'a', sizeof(char) * 1000);

  for (int i = 0; i < 10; i++) {
      printf("%i %lf %c\n", a[i], b[i], c[i]);
  }

  return 0;
}
