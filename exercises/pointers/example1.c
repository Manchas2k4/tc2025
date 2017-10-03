#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int a = 025;
	
	printf("decimal = %d\n", a);
	printf("octal = %o\n", a);
	printf("hexadecimal = %x\n", a);
	
	return 0;
}
	
