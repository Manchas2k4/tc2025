#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int i;
	
	printf("params = %i\n", argc);
	for (i = 0; i < argc; i++) {
		printf("args[%i] = %s\n", i, argv[i]);
	}
	return 0;
}
