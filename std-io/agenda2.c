#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME    256
#define MAX_PHONE   13
#define MAX_EMAIL   256

typedef unsigned char uchar;
typedef unsigned long ulong;

struct entry {
    char name[MAX_NAME];
    uchar len_name;
    char phone[MAX_PHONE];
    uchar len_phone;
    char email[MAX_EMAIL];
    uchar len_email;
    uchar deleted;
};

struct index {
  ulong pos;
  char name[MAX_NAME];
  uchar len_name;
};

struct index *indexes;
int next;

void serialize(struct entry *c) {
    c->len_name = strlen(c->name);
    c->name[c->len_name] = ' ';
    c->name[MAX_NAME - 1] = '\0';
    c->len_phone = strlen(c->phone);
    c->phone[c->len_phone] = ' ';
    c->phone[MAX_PHONE - 1] = '\0';
    c->len_email = strlen(c->email);
    c->email[c->len_email] = ' ';
    c->email[MAX_EMAIL - 1] = '\0';
}


void deserialize(struct entry *c) {
    c->name[c->len_name] = '\0';
    c->phone[c->len_phone] = '\0';
    c->email[c->len_email] = '\0';
}

void serialize_index(struct index *i) {
	i->len_name = strlen(i->name);
	i->name[i->len_name] = ' ';
	i->name[MAX_NAME - 1] = '\0';
}

void deserialize_index(struct index *i) {
	i->name[i->len_name] = '\0';
}

int compare(const void *a, const void *b) {
	struct index *left = (struct index *) a;
	struct index *right = (struct index *) b;
	
	return strcmp(left->name, right->name);
}

void insert(FILE *file, struct entry *c) {
	ulong pos;
	
	serialize(c);
	fseek(file, 0, SEEK_END);
	pos = ftell(file) / sizeof(struct entry);	
	
	if (fwrite(c, sizeof(struct entry), 1, file) == 1) {
		
		strcpy(indexes[next].name, c->name);
		indexes[next].pos = pos;
		indexes[next].len_name = c->len_name;
		next++;
		
		qsort(indexes, next, sizeof(struct index), compare);
		  
		printf("The record has been added.\n");
	} else {
		printf("There was an error, the record is not added.\n");
	}
}

void display(FILE *file, int show_deleted) {
	struct entry c;
	long i = 0, size = 0;

	printf("%-20s%-13s%-20s%-5s\n", "NAME", "PHONE", "EMAIL", "DEL");
	for (i = 0; i < next; i++) {
		fseek(file, (indexes[i].pos * sizeof(struct entry)) , SEEK_SET);
		fread(&c, sizeof(struct entry), 1, file);
		if (show_deleted || c.deleted == 0) {
			deserialize(&c);
			printf("%-20s%-13s%-20s%2i\n", c.name, c.phone, c.email,c.deleted);
		}
	}
}

ulong index_of(FILE *file, char *name) {
	struct index key, *result;
	
	strcpy(key.name, name);
	result = bsearch(&key, indexes, next, sizeof(struct index), compare);
	return (result == NULL)? -1 : result->pos;
}

void show(FILE *arch, char *name) {
    long pos = 0;
    struct entry c;
    
    pos = index_of(arch, name);
    if (pos == -1) {
        printf ("%s not found\n", name);
        return;
    }
    
    fseek(arch, (pos * sizeof(struct entry)), SEEK_SET);
    fread(&c, sizeof(struct entry), 1, arch);
    deserialize(&c);
    printf("%-20s%-13s%-20s%-5s\n", 
            "NAME", "PHONE", "EMAIL", "DEL");
    printf("%-20s%-13s%-20s%2i\n", 
            c.name, c.phone, c.email,c.deleted);
}

void update(FILE *arch, char *name, struct entry *c) {
    long pos;
    
    pos = index_of(arch, name);
    if (pos == -1) {
        printf("%s not found\n", name);
        return;
    }
    
    fseek(arch, (pos * sizeof(struct entry)), SEEK_SET);
    serialize(c);
    fwrite(c, sizeof(struct entry), 1, arch);
    printf("%s has been updated\n", name);
}

void erase(FILE *arch, char *name) {
    long pos;
    struct entry c;
    
    pos = index_of(arch, name);
    if (pos == -1) {
        printf("%s not found\n", name);
        return;
    }
    
    fseek(arch, (pos * sizeof(struct entry)), SEEK_SET);
    fread(&c, sizeof(struct entry), 1, arch);
    c.deleted = 1;
    fseek(arch, (-1 * sizeof(struct entry)), SEEK_CUR);
    fwrite(&c, sizeof(struct entry), 1, arch);
    printf("%s has been deleted\n", name);
}

void save_idx() {
	FILE *idx;
	
	idx = fopen("agenda.idx", "wb");
	fwrite(indexes, sizeof(struct index), next, idx);
	fclose(idx);
}

void load_idx(FILE * in) {
	fseek(in, 0, SEEK_END);
	next = ftell(in) / sizeof(struct index);
	
	indexes = (struct index*) malloc( sizeof(struct index) * (next + 100) );
	
	fseek(in, 0, SEEK_SET);
	fread(indexes, sizeof(struct index), next, in);
}

void init_idx() {
    FILE *idx;
	
	idx = fopen("agenda.idx", "rb+");
	if (!idx) {
	    indexes = malloc(sizeof(struct index) * 20);
	    next = 0;
	    
	} else {
	    load_idx(idx);
	}
}

void sesion1() {
    struct entry c;
	FILE *arch;
	
	printf("---------------------------\n");
	printf("----- INICIO SESION 1 -----\n");
	printf("---------------------------\n");
	
	arch = fopen("agenda.dat", "rb+");
	if (!arch) {
		arch = fopen("agenda.dat", "wb+");
	}
	init_idx();
	
	strcpy(c.name, "xanadu");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "xanadu@xanadu.com");
	insert(arch, &c);
	strcpy(c.name, "alba");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "alba@alba.com");
	insert(arch, &c);
	strcpy(c.name, "yoric");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "yoric@yoric.com");
	insert(arch, &c);
	strcpy(c.name, "carlos");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "carlos@carlos.com");
	insert(arch, &c);
	display(arch, 1);
	
	strcpy(c.name, "carlos");
	strcpy(c.phone, "1111111111");
	strcpy(c.email, "carlos@xanadu.com");
	update(arch, c.name, &c);
	display(arch, 1);
	
	strcpy(c.name, "no_existe");
	show(arch, c.name);
	strcpy(c.name, "yoric");
	show(arch, c.name);
	
	strcpy(c.name, "xanadu");
	erase(arch, c.name);
	display(arch, 1);
	
	/*
	    guardar los índices generados en 
	    esta sesión
	*/
	save_idx();
	
	fclose(arch);
	
	printf("---------------------------\n");
	printf("----- FIN    SESION 1 -----\n");
	printf("---------------------------\n");
}

void sesion2() {
    struct entry c;
	FILE *arch;
	
	printf("---------------------------\n");
	printf("----- INICIO SESION 2 -----\n");
	printf("---------------------------\n");
	
	arch = fopen("agenda.dat", "rb+");
	if (!arch) {
		arch = fopen("agenda.dat", "wb+");
	}
	init_idx();
	
	/*
	    verificar que se cargaron correcta-
	    mente los índices.
	*/
	display(arch, 1);
	
	
	strcpy(c.name, "daniel");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "daniel@daniel.com");
	insert(arch, &c);
	strcpy(c.name, "maria");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "maria@maria.com");
	insert(arch, &c);
	strcpy(c.name, "gonzalo");
	strcpy(c.phone, "1234567890");
	strcpy(c.email, "gonzalo@xanadu.com");
	insert(arch, &c);
	display(arch, 1);
	
	strcpy(c.name, "gonzalo");
	strcpy(c.phone, "1111111111");
	strcpy(c.email, "gonzalo@gonzalo.com");
	update(arch, c.name, &c);
	display(arch, 1);
	
	strcpy(c.name, "xanadu");
	show(arch, c.name);
	strcpy(c.name, "maria");
	show(arch, c.name);
	
	/*
	    guardar los índices generados en 
	    esta sesión
	*/
	save_idx();
	fclose(arch);
	
	printf("---------------------------\n");
	printf("----- FIN    SESION 2 -----\n");
	printf("---------------------------\n");
}

int main(int argc, char* argv[]) {
	sesion1();
	sesion2();
	return 0;
}
