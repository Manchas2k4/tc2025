#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	long *p;
	long x = 10; //, y = 1000;
	
	/*
	p = (long*) malloc(sizeof(long));
	if (!p) {
		perror(argv[0]);
		exit(0);
	}
	*/
	p = &x;
	printf("x = %li\n", x);
	printf("&x = %p\n", (&x));
	printf("p = %p\n", p);
	
	(*p) = 10;
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	(*p)++;
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	*(++p) = 11;
	//printf("x = %li\n", y);
	printf("p = %p\n", p);
	printf("*p = %li\n", (*p));
	
	//free(p);
	
	return 0;
}
	
