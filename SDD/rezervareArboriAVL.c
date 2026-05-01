#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Rezervare Rezervare;
struct Rezervare {
	unsigned int id;
	char* numeClient;
	int nrCamera;
	float sumaPlata;
};

Rezervare creareRezervare(unsigned int id, const char* numeClient, int nrCamera, float sumaPlata) {
	Rezervare r;
	r.id = id;
	r.numeClient = (char*)malloc(sizeof(char) * strlen(numeClient) + 1);
	strcpy(r.numeClient, numeClient);

	r.nrCamera = nrCamera;
	r.sumaPlata = sumaPlata;

	return r;
}

typedef struct Nod Nod;
struct Nod {
	Rezervare info;
	Nod* stanga;
	Nod* dreapta;
	int grad;
};

//functie de creare Nod
Nod* creareNod(Rezervare r, Nod* stanga, Nod* dreapta) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareRezervare(r.id, r.numeClient, r.nrCamera, r.sumaPlata);
	nou->stanga = stanga;
	nou->dreapta = dreapta;

	return nou;
}

//facem inserarea in arbore dupa id
Nod* inserareArbore(Nod* radacina, Rezervare r) {
	if (radacina) {
		if (r.id < radacina->info.id) {
			radacina->stanga = inserareArbore(radacina->stanga, r);

			return radacina;
		}
		else if (r.id > radacina->info.id) {
			radacina->dreapta = inserareArbore(radacina->dreapta, r);

			return radacina;
		}
		else {
			return radacina;
		}
	}
	return creareNod(r, NULL, NULL);
}

//am afisarea in pre ordine RSD
void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		printf("Grad: %d\n %u %s %d %.2f\n", radacina->grad, radacina->info.id, radacina->info.numeClient, radacina->info.nrCamera, radacina->info.sumaPlata);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

//recursivitatea
int maxim(int a, int b) {
	return a > b ? a : b;
}

//am calculat inaltimea arborelui
int nrNiveluri(Nod* radacina) {
	if (radacina) {
		return 1 + maxim(nrNiveluri(radacina->stanga), nrNiveluri(radacina->dreapta));
	}
	else {
		return 0;
	}
}

//calculam gradul de echilibru
void calculGrad(Nod* radacina) {
	if (radacina) {
		radacina->grad = (nrNiveluri(radacina->stanga) - nrNiveluri(radacina->dreapta));
		calculGrad(radacina->stanga);
		calculGrad(radacina->dreapta);
	}
}

Nod* rotireStanga(Nod* radacina) {
	Nod* x = radacina->dreapta;
	Nod* y = x->dreapta;

	x->stanga = radacina;
	radacina->dreapta = y;

	return x;
}

Nod* rotireDreapta(Nod* radacina) {
	Nod* x = radacina->stanga;
	Nod* y = x->stanga;

	x->dreapta = radacina;
	radacina->stanga = y;

	return x;
}

Nod* rotireStangaDreapta(Nod* radacina) {
	radacina->stanga = rotireStanga(radacina->stanga);
	return rotireDreapta(radacina);

}

Nod* rotireDreaptaStanga(Nod* radacina) {
	radacina->dreapta = rotireDreapta(radacina->dreapta);
	return rotireStanga(radacina);
}

void main() {

	Rezervare r;
	Nod* radacina = NULL;
	char buffer[70];
	char separator[] = ", ";
	char* token;
	FILE* f = fopen("rezervari.txt", "r");

	while (fgets(buffer, 70, f)) {
		token = strtok(buffer, separator);
		r.id = atoi(token);

		token = strtok(NULL, separator);
		r.numeClient = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(r.numeClient, token);

		token = strtok(NULL, separator);
		r.nrCamera = atoi(token);

		token = strtok(NULL, separator);
		r.sumaPlata = atof(token);

		radacina = inserareArbore(radacina, r);
		free(r.numeClient);
	}

	fclose(f);

	int inaltime = nrNiveluri(radacina);
	printf("Inaltime: %d\n", inaltime);
	calculGrad(radacina);
	afisarePreOrdine(radacina);
	printf("Afisare arbore dezechilibrat: \n");
	radacina = rotireStanga(radacina);
	calculGrad(radacina);
	afisarePreOrdine(radacina);

	printf("Afisare arbore dupa echilibrare: \n");
	afisarePreOrdine(radacina);

	//return 0;
}

