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



//functia de afisare
void afisareFarmacie(Farmacie farmacie) {
	printf("Nume: %s\n", farmacie.nume);
	printf("Suprafata: %.2f\n", farmacie.suprafata);
	printf("Numar angajati: %d\n", farmacie.nrAngajati);
}

//structura lista dubla
typedef struct ListaDubla AliasListaDubla;

struct ListaDubla {
	struct Farmacie farmacie;
	struct aliasNod* next;
	struct aliasNod* prev;
	int nrNoduri;
};

//functia de afisare a listei de farmacii de la inceput
void afisareListaFarmacieInceput(AliasListaDubla lista) {
	aliasNod* p = lista.first;
	while (p) {
		afisareFarmacie(p->farmacie);
		p = p->next;
	}
}

//functia de afisare a listei de farmacii de la sfarsit
void afisareListaFarmacieSfarsit(ListaDubla lista) {
	aliasNod* p = lista.last;
	while (p) {
		afisareFarmacie(p->farmacie);
		p = p->prev;
	}
}

//functie pt a adauga o farmacie la inceput
void adaugaLaInceputInLista(ListaDubla* lista, Farmacie farmacieNoua) {
	aliasNod* nou = (aliasNod*)malloc(sizeof(aliasNod)); //am adaugat un nod nou
	nou->farmacie = farmacieNoua; //shallow copy
	nou->next = NULL; //trecem la urmatorul nod
	nou->prev = lista->last; //ne intorceam la nodul precedent
	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

//functie de citire a elementelor din fisier
ListaDubla citireFarmaciiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaLaInceputInLista(&lista, f);
	}
	fclose(f);
	return lista;
}

void dezalocareLista(ListaDubla* lista) {
	aliasNod* p = lista->first;
	while (p) {
		aliasNod* aux = p;
		p = p->next;
		if (aux->farmacie.nume != NULL) {
			free(aux->farmacie.nume);
		}
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

int main() {
	ListaDubla lista = citireFarmaciiDinFisier("farmacieFisier.txt");
	afisareListaFarmacieInceput(lista);
	dezalocareLista(&lista);

	return 0;