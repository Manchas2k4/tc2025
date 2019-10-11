#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define FILEIN "file.in"
#define FILEOUT "file.out"
#define MYSTDIN 0
#define MYSTDOUT 1

int main(int argc, char* argv[]) {
	int inf, sup, i, acum;
	int fin, fout;

	fin = open(FILEIN, O_RDONLY);
	fout = open(FILEOUT, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	close(MYSTDIN);
	int aux = dup(fin);
	printf("INPUT = %i\n", aux);

	close(MYSTDOUT);
	aux = dup(fout);
	printf("OUTPUT = %i\n", aux);

	scanf("%i %i", &inf, &sup);
	while(inf != 0 && sup != 0) {
		acum = 0;
		for (i = inf; i <= sup; i++) {
			acum += (i % 2 == 0)? 1 : 0;
		}
		printf("%i\n", acum);
		scanf("%i %i", &inf, &sup);
	}
	return 0;
}
