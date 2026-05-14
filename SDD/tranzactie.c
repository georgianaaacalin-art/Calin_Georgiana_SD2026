#define _CRT_SECURE_NO_WARNIBGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

//structura Tranzactie
typedef struct Tranzactie Tranzactie;
struct Tranzactie {
	int cod;
	char* categorie;
	int luna;
	double valoare;
};

//structura Nod
typedef struct Nod Nod;
struct Nod {
	Tranzactie info;
	struct Nod* stanga;
	struct Nod* dreapta;
	int grad;
};

//functia de initializare/creare Tranzactie
Tranzactie initTranzactie(int cod, const char* categorie, int luna, double valoare) {
	Tranzactie t;
	t.cod = cod;
	t.categorie = (char*)malloc(strlen(categorie) + 1);
	strcpy(t.categorie, categorie);

	t.luna = luna;
	t.valoare = valoare;

	return t;
}

Nod* creareNod(Tranzactie t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initTranzactie(t.cod, t.categorie, t.luna, t.valoare);
	nou->stanga = NULL;
	nou->dreapta = NULL;
	nou->grad = 1;

	return nou;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltime(Nod* radacina) {
	if (radacina == NULL) { //daca nu avem arbore
		return 0; //iesim din functie
	}

	return 1 + maxim(calculeazaInaltime(radacina->stanga), calculeazaInaltime(radacina->dreapta));
}

void calculGrad(Nod* radacina) {
	if (radacina) {
		radacina->grad = calculeazaInaltime(radacina->stanga) - calculeazaInaltime(radacina->dreapta);
		calculGrad(radacina->stanga);
		calculGrad(radacina->dreapta);
	}
}

Nod* rotireLaStanga(Nod* x) {
	Nod* y = x->dreapta;
	Nod* t2 = y->stanga;
	y->stanga = x;
	x->dreapta = t2;

	return y;
}

Nod* rotireLaDreapta(Nod* y) {
	Nod* x = y->stanga;
	Nod* t2 = x->dreapta;
	x->dreapta = y;
	y->stanga = t2;

	return x;
}

int main() {

	return 0;
}





