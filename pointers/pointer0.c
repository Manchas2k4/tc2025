#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	long *p;
	long x = 10;
	
	/*
	p = (long*) malloc(sizeof(long));
	if (!p) {
		perror(argv[0]);
		exit(0);
	}
	*/
	p = &x;
	printf("x = %d\n", x);
	printf("&x = %p\n", (&x));
	printf("p = %p\n", p);
	
	(*p) = 10;
	printf("p = %p\n", p);
	printf("*p = %d\n", (*p));
	
	(*p)++;
	printf("p = %p\n", p);
	printf("*p = %d\n", (*p));
	
	*(++p) = 11;
	printf("p = %p\n", p);
	printf("*p = %d\n", (*p));
	
	//free(p);
	
	return 0;
}
	
