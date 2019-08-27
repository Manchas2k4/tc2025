#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int i;
	
	if (argc != 5) {
		printf("usage: %s arg1 arg2 arg3 arg4", argv[0]);
		return -1;
	}
	
	for (i = 0; i < argc; i++) {
		printf("argv[%i] = %s\n", i, argv[i]);
	}
	return 0;
}
