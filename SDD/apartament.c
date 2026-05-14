#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Apartament Apartament;
struct Apartament {
	int cod;
	char* proprietar;
	int camere;
	float pret;
};

typedef struct Nod Nod;
struct Nod {
	Apartament info;
	struct Nod* stanga;
	struct Nod* dreapta;
};

Apartament citireApartamentDinFisier(FILE* file) {
	char buffer[100];
	char separator[3] = ",\n";

	fgets(buffer, 100, file);
	char* auxiliar;
	Apartament a1;

	auxiliar = strtok(buffer, separator);
	a1.cod = atoi(auxiliar);

	auxiliar = strtok(NULL, separator);
	a1.proprietar = malloc(strlen(auxiliar) + 1);
	strcpy_s(a1.proprietar, strlen(auxiliar) + 1, auxiliar);

	auxiliar = strtok(NULL, separator);
	a1.camere = atoi(auxiliar);

	auxiliar = strtok(NULL, separator);
	a1.pret = atof(auxiliar);

	return a1;
}

void afisareApartament(Apartament apartament) {
	printf("\nCodul este: %d", apartament.cod);
	printf("\nProprietarul este: %s", apartament.proprietar);
	printf("\nAre %d camere.", apartament.camere);
	printf("\nPretul este: %.2f\n", apartament.pret);
}

void adaugaApartamentInArbore(Nod** radacina, Apartament apartamentNou) {
	if (*radacina == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = apartamentNou;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.cod > apartamentNou.cod) {
			adaugaApartamentInArbore(&((*radacina)->stanga), apartamentNou);
		}
		if ((*radacina)->info.cod < apartamentNou.cod) {
			adaugaApartamentInArbore(&((*radacina)->dreapta), apartamentNou);
		}
	}
}

Nod* citireArboreDeAparatamenteDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Apartament a = citireApartamentDinFisier(f);
			adaugaApartamentInArbore(&radacina, a);
		}
	}
	fclose(f);
	return radacina;
}

void afisareApartamenteDinArbore(Nod* radacina) { //SRD
	if (radacina) {
		afisareApartamenteDinArbore(radacina->stanga);
		afisareApartament(radacina->info);
		afisareApartamenteDinArbore(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareApartament(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

void dezalocareArboreDeApartamente(Nod** radacina) {
	if (radacina) {
		dezalocareArboreDeApartamente(&(*radacina)->stanga);
		dezalocareArboreDeApartamente(&(*radacina)->dreapta);
		free((*radacina)->info.proprietar);
		free(*radacina);
		*radacina = NULL;
	}
}

Apartament getApartamentByCod(Nod* radacina, int cod) {
	Apartament a;
	a.cod = -1;
	if (radacina) {
		if (radacina->info.cod == cod) {
			a = radacina->info;
			a.proprietar = malloc(sizeof(char) * (strlen(radacina->info.proprietar) + 1));
			strcpy(a.proprietar, radacina->info.proprietar);
		}
		if (cod < radacina->info.cod) {
			a = getApartamentByCod(radacina->stanga, cod);
		}
		if (cod > radacina->info.cod) {
			a = getApartamentByCod(radacina->dreapta, cod);
		}
	}
	return a;
}

int determinaNrNoduri(Nod* radacina) {
	if (radacina) {
		return determinaNrNoduri(radacina->stanga) + determinaNrNoduri(radacina->dreapta) + 1;
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
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) +
			calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretulApartamentelorUnuiProprietar(Nod* radacina, const char* proprietar) {
	if (radacina) {
		float suma = calculeazaPretulApartamentelorUnuiProprietar(radacina->stanga, proprietar) +
			calculeazaPretulApartamentelorUnuiProprietar(radacina->dreapta, proprietar);
		if (strcmp(radacina->info.proprietar, proprietar) == 0) {
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;

}


int main() {
	Nod* radacina = citireArboreDeAparatamenteDinFisier("apartamente.txt");
	afisarePreOrdine(radacina);

	printf("\nApartamentul cautat: ");
	afisareApartament(getApartamentByCod(radacina, 232));

	printf("\nNumar noduri: %d", determinaNrNoduri(radacina));

	printf("\nInaltime arbore: %d", calculeazaInaltimeArbore(radacina));

	printf("\nPret total: %.2f", calculeazaPretTotal(radacina));

	printf("\nPretul maxim este: %.2f", calculeazaPretulApartamentelorUnuiProprietar(radacina, "Andrei Ionescu"));

	dezalocareArboreDeApartamente(&radacina);

	return 0;
}