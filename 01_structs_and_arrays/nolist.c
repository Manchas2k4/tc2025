#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef enum {NONE, INTEGER, DOUBLE} Type;

typedef union {
	int valInt;
	double valDouble;
} Value;

typedef struct {
	Type type;
	Value value;
} NodeValue;

typedef struct {
	NodeValue nv;
	int next;
} Node;

typedef struct {
	int free, used;
	Node data[SIZE];
} List;

void init(List *lst) {
	int i;
	
	lst->free = 0;
	lst->used = -1;
	for (i = 0; i < SIZE; i++) {
		lst->data[i].nv.type = NONE;
		lst->data[i].nv.value.valDouble = 0;
		lst->data[i].next = i + 1;
	}
	lst->data[SIZE - 1].next = -1;
}

void traverse(List *lst) {
	int p;
	
	printf("USED: ");
	p = lst->used;
	while (p != -1) {
		printf("(pos: %i, ", p);
		if (lst->data[p].nv.type == INTEGER) {
			printf("int: %i) ", lst->data[p].nv.value.valInt);
		} else {
			printf("double: %.2lf) ", lst->data[p].nv.value.valDouble);
		}
		p = lst->data[p].next;
	}
	printf("\n");
	
	printf("FREE: ");
	p = lst->free;
	while (p != -1) {
		printf("(pos: %i) ", p);
		p = lst->data[p].next;
	}
	printf("\n");
}

/*
	lst->data[p].nv.type = type;
	if (type == INTEGER) {
		lst->data[p].nv.value.valInt = *((int*) val);
	} else {
		lst->data[p].nv.value.valDouble = *((double*) val);
	}
	lst->data[p].next = lst->used;
	lst->used = p;
	return 1;
*/

double convert(NodeValue *nv) {
	return (nv->type == INTEGER)? ((double) nv->value.valInt) : nv->value.valDouble;
}

int add(List *lst, Type type, void* val) {
	int p, q, r;
	
	if (lst->free == -1) {
		printf("there are no free nodes\n");
		return 0;
	}
	
	p = lst->free;
	lst->free = lst->data[lst->free].next;
	
	NodeValue nv;
	nv.type = type;
	if (nv.type == INTEGER) {
		nv.value.valInt = *( (int*) val );
	} else {
		nv.value.valDouble = *( (double*) val );
	}
	
	r = -1;
	q = lst->used;
	while (q != -1 && convert(&(lst->data[q].nv)) < convert(&nv)) {
		r = q;
		q = lst->data[q].next;
	}
	
	lst->data[p].nv = nv;
	if (r == -1) {
		lst->data[p].next = lst->used;
		lst->used = p;
	} else {
		lst->data[r].next = p;
		lst->data[p].next = q;
	}
}

NodeValue* del(List *lst) {
	int p;
	
	if (lst->used == -1) {
		return NULL;
	}
	
	p = lst->used;
	lst->used = lst->data[lst->used].next;
	
	lst->data[p].next = lst->free;
	lst->free = p;
	
	NodeValue* nv = (NodeValue*) malloc(sizeof(NodeValue));
	*nv = lst->data[p].nv;
	return nv;
}

int main(int argc, char* argv[]) {
	List myList;
	int valInt;
	double valDouble;
	
	init(&myList);
	traverse(&myList);
	printf("\n");
	
	valInt = 10;
	add(&myList, INTEGER, (void*) &valInt);
	printf("adding 10...\n");
	traverse(&myList);
	printf("\n");
	
	valDouble = 15.0;
	add(&myList, DOUBLE, (void*) &valDouble);
	printf("adding 15.0...\n");
	traverse(&myList);
	printf("\n");
	
	valInt = 12;
	add(&myList, INTEGER, (void*) &valInt);
	printf("adding 12...\n");
	traverse(&myList);
	printf("\n");
	
	del(&myList);
	printf("removing...\n");
	traverse(&myList);
	printf("\n");
	
	valDouble = 9.0;
	add(&myList, DOUBLE, (void*) &valDouble);
	printf("adding 9.0...\n");
	traverse(&myList);
	printf("\n");
	
	valInt = 14;
	printf("adding 14...\n");
	add(&myList, INTEGER, (void*) &valInt);
	traverse(&myList);
	printf("\n");
	
	del(&myList);
	printf("removing...\n");
	traverse(&myList);
	printf("\n");
	
	valDouble = 20.0;
	printf("adding 20.0...\n");
	add(&myList, DOUBLE, (void*) &valDouble);
	traverse(&myList);
	printf("\n");
	
	del(&myList);
	printf("removing...\n");
	traverse(&myList);
	printf("\n");
	
	del(&myList);
	printf("removing...\n");
	traverse(&myList);
	printf("\n");
	
	valDouble = 5.0;
	printf("adding 5.0...\n");
	add(&myList, DOUBLE, (void*) &valDouble);
	traverse(&myList);
	printf("\n");
	
	valInt = 2;
	printf("adding 2...\n");
	add(&myList, INTEGER, (void*) &valInt);
	traverse(&myList);
	printf("\n");
	
	valInt = 4;
	printf("adding 4...\n");
	add(&myList, INTEGER, (void*) &valInt);
	traverse(&myList);
	printf("\n");
	return 0;
}