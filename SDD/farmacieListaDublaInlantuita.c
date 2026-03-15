#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Farmacie AliasFarmacie; //aliasul

//structura farmaciei
struct Farmacie {
	char* nume;
	float suprafata;
	int nrAngajati;
};

//functia de initializare
struct Farmacie initializareFarmacie(const char* nume, float suprafata, int nrAngajati) {
	struct Farmacie f;
	f.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(f.nume, nume);
	f.suprafata = suprafata;
	f.nrAngajati = nrAngajati;

	return f;

}

typedef struct Nod aliasNod;

// creare structura Nod
struct Nod {
	struct Farmacie aliasFarmacie;
	struct aliasNod* next;
	struct aliasNod* prev;

};

//citire din fisier
struct Farmacie citireFisier(FILE* f) {
	char buffer[60];
	struct Farmacie fa = { NULL, 0.0, 0 };
	if (fgets(buffer, sizeof(buffer), f)) {
		char* token = strtok(buffer, ",;\n");
		if (token == NULL) {
			return fa;
		}
		fa.nume = (char*)malloc(strlen(token) + 1);
		strcpy(fa.nume, token);

		token = strtok(NULL, ",;\n");
		if (token == NULL) {
			return fa;
		}
		fa.suprafata = (float)atof(token);

		token = strtok(NULL, ",;\n");
		if (token = NULL) {
			return fa;
		}
		fa.nrAngajati = (int)atoi(token);
	}

	return fa;

}



int main() {

	return 0;
}
