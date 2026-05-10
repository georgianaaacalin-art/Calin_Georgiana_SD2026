#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Motocicleta Motocicleta;
struct Motocicleta {
	int id;
	int nrRoti;
	float pret;
	char* model;
	char* numeClient;
	unsigned char serie;
};

typedef struct Nod Nod;
struct Nod {
	Motocicleta info;
	Nod* stanga;
	Nod* dreapta;
};

void afisareMotocicleta(Motocicleta motocicleta) {
	printf("Id: %d\n", motocicleta.id);
	printf("Numar roti: %d\n", motocicleta.nrRoti);
	printf("Pret: %.2f\n", motocicleta.pret);
	printf("Model: %s\n", motocicleta.model);
	printf("Nume client: %s\n", motocicleta.numeClient);
	printf("Serie: %c\n", motocicleta.serie);
}

//functie de initializare motocicleta
Motocicleta initializareMotocicleta(int id, int nrRoti, float pret, const char* model, const char* numeClient, unsigned char serie) {
	Motocicleta m;
	m.id = id;
	m.nrRoti = nrRoti;
	m.pret = pret;

	m.model = (char*)malloc(sizeof(char) * strlen(model) + 1);
	strcpy(m.model, model);

	m.numeClient = (char*)malloc(sizeof(char) * strlen(numeClient) + 1);
	strcpy(m.numeClient, numeClient);

	m.serie = serie;

	return m;
}

int calculDiferentaInaltimi(Nod* radacina) {
	if (radacina != NULL) {
		return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
	}
	else {
		return 0;
	}
}

void rotireLaStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireLaDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}


//adauga motocicleta in arbore dupa id
void adaugaMotocicletaInArbore(Nod** radacina, Motocicleta motocicletaNoua) {
	if (*radacina == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = motocicletaNoua;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.id > motocicletaNoua.id) {
			adaugaMotocicletaInArbore(&((*radacina)->stanga), motocicletaNoua);
		}
		if ((*radacina)->info.id < motocicletaNoua.id) {
			adaugaMotocicletaInArbore(&((*radacina)->dreapta), motocicletaNoua);
		}
	}

	int diferentaInaltimi = calculDiferentaInaltimi(*radacina);
	if (diferentaInaltimi == 2) {
		//dezechilibru pe partea stanga
		if (calculDiferentaInaltimi((*radacina)->stanga) == -1) {
			rotireLaStanga(&(*radacina)->stanga);
		}
		rotireLaDreapta(radacina);
	}
	if (diferentaInaltimi == -2) {
		//dezechilibru pe partea dreapta
		if (calculDiferentaInaltimi((*radacina)->dreapta) == 1) {
			rotireLaDreapta(&(*radacina)->dreapta);
		}
		rotireLaStanga(radacina);
	}
}

void afisareMotocicleteDinArbore(Nod* radacina) {
	if (radacina) {
		afisareMotocicleteDinArbore(radacina->stanga);
		afisareMotocicleta(radacina->info);
		afisareMotocicleteDinArbore(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) { //RSD
	if (radacina) {
		afisareMotocicleta(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

void dezalocareArboreDeMotociclete(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMotociclete(&(*radacina)->stanga);
		dezalocareArboreDeMotociclete(&(*radacina)->dreapta);
		free((*radacina)->info.model);
		free((*radacina)->info.numeClient);
		free(*radacina);
		*radacina = NULL;
	}
}

Motocicleta getMotocicletaById(Nod* radacina, int id) {
	Motocicleta m;
	m.id = -1;
	if (radacina) {
		if (radacina->info.id == id) {
			m = radacina->info;
			m.model = malloc(sizeof(char) * (strlen(radacina->info.model) + 1));
			strcpy(m.model, radacina->info.model);
			m.numeClient = malloc(sizeof(char) * strlen(radacina->info.numeClient) + 1);
			strcpy(m.numeClient, radacina->info.numeClient);
		}
		if (id < radacina->info.id) {
			m = getMotocicletaById(radacina->stanga, id);
		}
		if (id > radacina->info.id) {
			m = getMotocicletaById(radacina->dreapta, id);
		}
	}
	return m;
}

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
		return maxim(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta)) + 1;
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretulMotocicletelorUnuiClient(Nod* radacina, const char* numeClient) {
	if (radacina) {
		float suma = calculeazaPretulMotocicletelorUnuiClient(radacina->stanga, numeClient) + calculeazaPretulMotocicletelorUnuiClient(radacina->dreapta, numeClient);
		if (strcmp(radacina->info.numeClient, numeClient) == 0) {
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
}




int main() {

	Nod* radacina = NULL;
	adaugaMotocicletaInArbore(&radacina, initializareMotocicleta(1, 2, 5500.99, "Harley", "Ionescu", 'A'));
	adaugaMotocicletaInArbore(&radacina, initializareMotocicleta(2, 3, 6500.99, "MotoMoto", "Georgescu", 'B'));
	adaugaMotocicletaInArbore(&radacina, initializareMotocicleta(3, 4, 7500.99, "Moto", "Vasilescu", 'C'));
	afisarePreOrdine(radacina);

	printf("\nMasina cautata: ");
	afisareMotocicleta(getMotocicletaById(radacina, 2));


	printf("\nNumar noduri: %d\n", determinaNumarNoduri(radacina));
	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("\nPret total:%.2f\n", calculeazaPretTotal(radacina));
	printf("\nSuma preturilor motocicletelor unui client: %.2f\n", calculeazaPretulMotocicletelorUnuiClient(radacina, "Georgescu"));

	dezalocareArboreDeMotociclete(&radacina);

	return 0;
}