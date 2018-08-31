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

void insert(FILE* file, struct entry *c) {
	serialize(c);
	fseek(file, 0, SEEK_END);
	if (fwrite(c, sizeof(struct entry), 1, file) == 1) {
		printf("The record has been added.\n");
	} else {
		printf("There was an error, the record is not added.\n");
	}
}

void display(FILE* file, int show_deleted) {
	struct entry c;
	long pos = 0, size = 0;

	printf("%-20s%-13s%-20s%-5s\n", "NAME", "PHONE", "EMAIL", "DEL");
	fseek(file, 0, SEEK_END);
	size = ftell(file) / sizeof(struct entry);
	fseek(file, 0, SEEK_SET);
	for (pos = 0; pos < size; pos++) {
		fread(&c, sizeof(struct entry), 1, file);
		if (show_deleted || c.deleted == 0) {
			deserialize(&c);
			printf("%-20s%-13s%-20s%2i\n", c.name, c.phone, c.email,c.deleted);
		}
	}
}

int index_of(FILE* file, char* name) {
	long pos = 0, size = 0;
	struct entry c;
	
	fseek(file, 0, SEEK_END);
	size = ftell(file) / sizeof(struct entry);
	fseek(file, 0, SEEK_SET);
	for (pos = 0; pos < size; pos++) {
		fread(&c, sizeof(struct entry), 1, file);
		deserialize(&c);
		if (strcmp(c.name, name) == 0) {
			return pos;
		}
	}
	return -1;
}	

void search(FILE* file, char* name) {
	long pos = 0;
	struct entry c;
	
	pos = index_of(file, name);
	if (pos == -1) {
		printf("%s is not in the file.\n", name);
	} else {
		fseek(file, (pos * sizeof(struct entry)), SEEK_SET);
		fread(&c, sizeof(struct entry), 1, file);
		deserialize(&c);
		if (!c.deleted) {
			printf("%-20s%-13s%-20s%-5s\n", "NAME", "PHONE", "EMAIL", "DEL");
			printf("%-20s%-13s%-20s%2i\n", c.name, c.phone, c.email,c.deleted);
		} else {
			printf("%s is marked as deleted.\n", name);
		}
	}
}

void erase(FILE* file, char* name) {
	long pos;
	struct entry c;
	
	pos = index_of(file, name);
	if (pos == -1) {
		printf("%s is not in the file.\n", name);
	} else {
		fseek(file, (pos * sizeof(struct entry)), SEEK_SET);
		fread(&c, sizeof(struct entry), 1, file);
		if (!c.deleted) {
			c.deleted = 1;
			fseek(file, (-1 * sizeof(struct entry)), SEEK_CUR);
			fwrite(&c, sizeof(struct entry), 1, file);
			printf("%s has been deleted.\n", name);
		}
	}
}

void update(FILE* file, char* name, struct entry *new_value) {
	long pos;
	
	pos = index_of(file, name);
	if (pos == -1) {
		printf("%s is not in the file.\n", name);
	} else {
		fseek(file, (pos * sizeof(struct entry)), SEEK_SET);
		serialize(new_value);
		fwrite(new_value, sizeof(struct entry), 1, file);
		printf("The record has been modified.\n");
	}
}

void menu() {
	printf("a. insert\n");
	printf("b. erase\n");
	printf("c. update\n");
	printf("d. display\n");
	printf("e. exist\n");
	printf("op? ");
}

int main(int argc, char* argv[]) {
	char op, nombre[MAX_NAME];
	struct entry c;
	FILE *file;
	
	file = fopen("agenda.dat", "rb+");
	if (!file) {
		file = fopen("agenda.dat", "wb+");
	}
	
	do {
		menu();
		op = getc(stdin);
		getc(stdin);
		switch (op) {
			case 'a':
			case 'A':
						printf("name: ");
						fgets(c.name, MAX_NAME - 1, stdin);
						printf("phone: ");
						fgets(c.phone, MAX_PHONE - 1, stdin);
						printf("email: ");
						fgets(c.email, MAX_EMAIL - 1, stdin);
						c.deleted = 0;
						insert(file, &c);
						break;
			case 'b':
			case 'B':
						printf("name: ");
						fgets(c.name, MAX_NAME - 1, stdin);
						erase(file, c.name);
						break;
			case 'c':
			case 'C':
						printf("name: ");
						fgets(c.name, MAX_NAME - 1, stdin);
						printf("new phone: ");
						fgets(c.phone, MAX_PHONE - 1, stdin);
						printf("new email: ");
						fgets(c.email, MAX_EMAIL - 1, stdin);
						c.deleted = 0;
						update(file, c.name, &c);
						break;
			case 'd':
			case 'D':
						display(file, 1);
						break;
			case 'e':
			case 'E':
						printf("Quiting...\n");
						break;
			default :
						printf("Unknown option.\n");
						break;
		}
	} while (op != 'e' && op != 'E');
	fclose(file);
	return 0;
}