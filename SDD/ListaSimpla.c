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

	return a;
}

typedef struct Nod nod;
struct Nod {
	alias info;
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

nod* inserareSfarsit(nod* cap, alias a) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFruct(a.nume, a.greutate, a.nrFructe);
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		nod* aux = cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	return cap;
}

void afisare(nod* cap) {
	nod* aux = cap;
	while (aux) {
		printf("%s, %.2f, %d\n", aux->info.nume, aux->info.greutate, aux->info.nrFructe);
		aux = aux->next;
	}
}

void dezalocare(nod* cap) {
	nod* aux = cap;
	free(aux->info.nume);
	nod* temp = aux->next;
	free(aux);
	aux = temp;
}

void main() {
	alias a;
	nod* cap = NULL;
	char buffer[50];
	int numar;

	FILE* f = fopen("fruct.txt", "r");
	fscanf(f, "%d", &numar);

	for (int i = 0; i < numar; i++) {
		fscanf(f, "%s", buffer);
		a.nume = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy(a.nume, buffer);

		fscanf(f, "%.2f", &a.greutate);
		fscanf(f, "%d", &a.nrFructe);
	}

	fclose(f);
	afisare(cap);
	dezalocare(cap);
}

