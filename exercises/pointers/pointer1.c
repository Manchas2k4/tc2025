#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000000

int main(int argc, char* argv[]) {
	int *ptr, var;
	
	var = 10;
	ptr = &var;
	printf("loc. de var = %p valor de var = %i\n", &var, var);
	printf("loc. a la cual apunta ptr = %p el valor almacenado ptr = %i\n", ptr, (*ptr));
	
	var = var + 1;
	(*ptr) = (*ptr) + 1;
	printf("loc. de var = %p valor de var = %i\n", &var, var);
	printf("loc. a la cual apunta ptr = %p el valor almacenado ptr = %i\n", ptr, (*ptr));
	
	ptr = ptr + 1;
	printf("loc. de var = %p valor de var = %i\n", &var, var);
	printf("loc. a la cual apunta ptr = %p el valor almacenado ptr = %i\n", ptr, (*ptr));
	
	return 0;
}
