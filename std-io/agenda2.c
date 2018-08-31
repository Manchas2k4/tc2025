#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME    256
#define MAX_PHONE   13
#define MAX_EMAIL   256

typedef unsigned char uchar;

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
  long pos;
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

/*
    Una operación para serializar y deseria-
    lizar una estructura tipo índice. Esta 
    función se utilizará al momento de guar-
    dar los índices en un arreglo.
*/
void serialize_index(struct index *i) {
}

void deserialize_index(struct index *i) {
}

/* 
	Agregar una función de comparación de
	indexes usando el nombre
*/
int compare(const void *a, const void *b) {
	return -1;
}

/*
    Agregar:
        a) antes de escribir obtenga la posi-
        ción en que se va a escribir.
        b) escribir el registro.
        c) agregar un nuevo índice al arreglo
        usando la posición obtenida y el nom-
        bre del contacto.
        d) ordenar el arreglo usando qsort.
*/
void insert(FILE *arch, struct entry *c) {
}

/*
    Modificar el display para que muestre
    siempre los elementos ordenados de menor
    a mayor. TIP: usar el arreglo de índices
    para desplegar la información.
*/
void display(FILE *arch, int show_deleted) {
}

/*
    Modificar el index_of para hacer utilizar 
    bsearch (búsqueda binaria) en el arre----
    glo de índices y regresar la posición que 
    ese nombre debe tener en el archivo. En 
    caso de no existir o que el registro está 
    marcado como borrado, regresa -1.
*/
long index_of(FILE *arch, char *name) {
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

/*
    agregar una función que se encarga de guardar
    todos los índices en un archivo llamado 
    agenda.idx. Recuerda cerrar el archivo al 
    final.
*/
void save_idx() {
}

/*
    agregar una función que se encarga de cargar
    los índices que se hallan en archivo 
    agenda.idx. La función recibe un FILE*
    del archivo ya abierto. Esta función deberá 
    calcular el número de registros que hay en el 
    archivo, dimensionar en apuntador (estructura
    del arreglo de índices) al tamaño + 10, y
    guardar cada uno de esos registros en el 
    arreglo. 
*/
void load_idx(FILE * in) {
}

void init_idx() {
    FILE *idx;
	
	idx = fopen("agenda.idx", "rb+");
	if (!idx) {
	    /*
	    dimensiona el apuntador (estructura
	    del arreglo de índices) a un tamaño 
	    de 20. La posición inicial es 0.
	    */
	    indexes = malloc(sizeof(struct index) * 20);
	    next = 0;
	    
	} else {
	    load_idx(idx);
	    /*
	    invoca al a función que carga los 
	    indices el archivo de índices.
	    */
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
