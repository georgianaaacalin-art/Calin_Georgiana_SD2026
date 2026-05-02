#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct Bicicleta Bicicleta;
struct Bicicleta {
	int id;
	int nrRoti;
	float pret;
	char* model;
};

typedef struct Nod Nod;
struct Nod {
	struct Bicicleta info;
	struct Nod* dreapta;
	struct Nod* stanga;
};

//functie de afisare bicicleta
void afisareBicicleta(Bicicleta bicicleta) { //functie de afisare bicileta
	printf("Id: %d\n", bicicleta.id);
	printf("Numar roti: %d\n", bicicleta.nrRoti);
	printf("Pret: %.2f\n", bicicleta.pret);
	printf("Model: %s\n", bicicleta.model);
}

void afisareVectorBiciclete(Bicicleta* biciclete, int nrBiciclete) {
	for (int i = 0; i < nrBiciclete; i++) {
		afisareBicicleta(biciclete[i]);
	}
}

//functie de adaugare ibicicleta in vector
void adaugaBicicletaInVector(Bicicleta** biciclete, int* nrBiciclete, Bicicleta bicicletaNoua) {
	Bicicleta* aux = (Bicicleta*)malloc(sizeof(Bicicleta) * ((*nrBiciclete) + 1));
	for (int i = 0; i < *nrBiciclete; i++) {
		aux[i] = (*biciclete)[i];
	}
	aux[(*nrBiciclete)] = bicicletaNoua; //shalow copy
	free(*biciclete);
	(*biciclete) = aux;
	(*nrBiciclete)++;

}

Bicicleta citireBicicletaDinFisier(FILE* file) {
	Bicicleta b1;

	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;

	aux = strtok(buffer, sep);
	b1.id = atoi(aux);
	b1.nrRoti = atoi(strtok(NULL, sep));
	b1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	b1.model = malloc(strlen(aux) + 1);
	strcpy_s(b1.model, strlen(aux) + 1, aux);

	return b1;
}

//functie de adaugare a unei biciclete in arbore dupa id
void adaugaBicicletaInArbore(Nod** radacina, Bicicleta bicicletaNoua) {
	if (*radacina == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = bicicletaNoua;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.id > bicicletaNoua.id) {
			adaugaBicicletaInArbore(&((*radacina)->stanga), bicicletaNoua);
		}
		if ((*radacina)->info.id < bicicletaNoua.id) {
			adaugaBicicletaInArbore(&((*radacina)->dreapta), bicicletaNoua);
		}
	}
}

Nod* citireArboreDeBicicleteDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Bicicleta b = citireBicicletaDinFisier(f);
			adaugaBicicletaInArbore(&radacina, b);
		}
	}
	fclose(f);
	return radacina;
}

void afisareBicicleteDinArbore(Nod* radacina) {
	if (radacina) {
		afisareBicicleteDinArbore(radacina->stanga);
		afisareBicicleta(radacina->info);
		afisareBicicleteDinArbore(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) { //RSD
	if (radacina) {
		afisareBicicleta(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

void afisareInOrdine(Nod* radacina) { //SRD
	if (radacina) {
		afisarePreOrdine(radacina->stanga);
		afisareBicicleta(radacina->info);
		afisarePreOrdine(radacina->dreapta);
	}
}

void afisarePostOrdine(Nod* radacina) { //SDR
	if (radacina) {
		afisarePostOrdine(radacina->stanga);
		afisarePostOrdine(radacina->dreapta);
		afisareBicicleta(radacina->info);
	}
}

void dezalocareArboreDeBiciclete(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeBiciclete(&(*radacina)->stanga);
		dezalocareArboreDeBiciclete(&(*radacina)->dreapta);
		free((*radacina)->info.model);
		free(*radacina);
		*radacina = NULL;
	}
}

//functie de afisare a unei biciclete dupa id
Bicicleta getBicicletaById(Nod* radacina, int id) {
	Bicicleta b;
	b.id = -1;
	if (radacina) {
		if (radacina->info.id == id) {
			b = radacina->info;
			b.model = malloc(sizeof(char) * (strlen(radacina->info.model) + 1));
			strcpy(b.model, radacina->info.model);
		}
		if (id < radacina->info.id) {
			b = getBicicletaById(radacina->stanga, id);
		}
		if (id > radacina->info.id) {
			b = getBicicletaById(radacina->dreapta, id);
		}
	}
	return b;
}

//functie de numarat npdurile unui arbore
int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta) + 1;
	}
	return 0;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		return maxim(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta) + 1);
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretulBicicletelorCu2Roti(Nod* radacina, float pret) {
	if (radacina) {
		float suma = calculeazaPretulBicicletelorCu2Roti(radacina->stanga, pret) + calculeazaPretulBicicletelorCu2Roti(radacina->dreapta, pret);
		if (pret == 0) {
			suma += pret;
		}
		return suma;
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeBicicleteDinFisier("biciclete.txt");
	afisarePreOrdine(radacina);

	printf("\nMasina cautata: \n");
	afisareBicicleta(getBicicletaById(radacina, 6));

	printf("\nNumar noduri: %d\n", determinaNumarNoduri(radacina));

	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));

	printf("\nPret total: %.2f\n", calculeazaPretTotal(radacina));

	printf("\nSuma preturilor bicicletelor cu 2 roti: %.2f\n", calculeazaPretulBicicletelorCu2Roti(radacina, 2));

	dezalocareArboreDeBiciclete(&radacina);


	return 0;
}

