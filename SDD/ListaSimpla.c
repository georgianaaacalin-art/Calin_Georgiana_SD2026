#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Fruct alias;

struct Fruct {
	char* nume;
	float greutate;
	int nrFructe;
};

//Lista simpla
alias creareFruct(const char* nume, float greutate, int nrFructe) {
	alias a;
	a.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
	strcpy(a.nume, nume);
	a.greutate = greutate;
	a.nrFructe = nrFructe;
}

typedef struct Nod nod;
struct Nod {
	nod info;
	struct nod* next;
};

nod* inserareInceput(nod* cap, alias a) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		nou->next = cap;
		cap = nou;
	}

	return cap;
}

