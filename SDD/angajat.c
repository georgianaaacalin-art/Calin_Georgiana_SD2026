#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Angajat Angajat;
struct Angajat {
	int id;
	char* nume;
	float salariu;
};

typedef struct Nod Nod;
struct Nod {
	Angajat info;
	struct Nod* dreapta;
	struct Nod* stanga;
};

Angajat citireAngajatDinFisier(FILE* file) {
	char buffer[100]; 
	char separator[3] = ",\n";
	fgets(buffer, 100, file); 
	char* auxiliar; 
	Angajat a1; 
	auxiliar = strtok(buffer, separator);
	a1.id = atoi(auxiliar); 

	auxiliar = strtok(NULL, separator); 
	a1.nume = malloc(sizeof(char) * (strlen(auxiliar) + 1)); 
	strcpy_s(a1.nume, strlen(auxiliar) + 1, auxiliar); 

	a1.salariu = atof(strtok(NULL, separator)); 

	return a1; 
}

void afisareAngajat(Angajat angajat) {
	printf("\nId ul este: %d\n", angajat.id);
	printf("Numele este: %s\n", angajat.nume);
	printf("Salariul este: %.2f\n", angajat.salariu);
}

void adaugaAngajatInArbore(Nod** radacina, Angajat angajatNou) {
	if (*radacina == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = angajatNou;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.id > angajatNou.id) {
			adaugaAngajatInArbore(&((*radacina)->stanga), angajatNou);
		}
		if ((*radacina)->info.id < angajatNou.id) {
			adaugaAngajatInArbore(&((*radacina)->dreapta), angajatNou);
		}
	}
}

Nod* citireArboreDeAngajatidinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Angajat a = citireAngajatDinFisier(f);
			adaugaAngajatInArbore(&radacina, a);
		}
	}
	fclose(f);
	return radacina;
}

void afisareAngajatiDinArbore(Nod* radacina) {
	if (radacina) {
		afisareAngajatiDinArbore(radacina->stanga);
		afisareAngajat(radacina->info);
		afisareAngajatiDinArbore(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareAngajat(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

void dezalocareArboreDeAngajati(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeAngajati(&(*radacina)->stanga);
		dezalocareArboreDeAngajati(&(*radacina)->dreapta);
		free((*radacina)->info.nume);
		free(*radacina);
		*radacina = NULL;
	}
}

//functie care cauta in arbore un angajat dupa id
Angajat getAngajatById(Nod* radacina, int id) {
	Angajat a;
	a.id = -1;
	if (radacina) {
		if (radacina->info.id == id) {
			a = radacina->info;
			a.nume = malloc(sizeof(char) * (strlen(radacina->info.nume) + 1));
			strcpy(a.nume, radacina->info.nume);
		}
		if (id < radacina->info.id) {
			a = getAngajatById(radacina->stanga, id);
		}
		if (id > radacina->info.id) {
			a = getAngajatById(radacina->dreapta, id);
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
		return maxim(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta) + 1);
	}
	return 0;
}

float calculeazaTotalSalarii(Nod* radacina) {
	if (radacina) {
		return radacina->info.salariu + calculeazaTotalSalarii(radacina->stanga) + calculeazaTotalSalarii(radacina->dreapta);
	}
	return 0;
}

float calculeazaSalariulTotalAlUnuiAngajat(Nod* radacina, const char* numeAngajat) {
	if (radacina) {
		float suma = calculeazaSalariulTotalAlUnuiAngajat(radacina->stanga, numeAngajat) +
			calculeazaSalariulTotalAlUnuiAngajat(radacina->dreapta, numeAngajat);

		if (strcmp(radacina->info.nume, numeAngajat) == 0) {
			suma += radacina->info.salariu;
		}

		return suma;
	}

	return 0;
}


int main() {
	Nod* radacina = citireArboreDeAngajatidinFisier("angajati.txt");
	afisarePreOrdine(radacina);

	printf("\nAngajatul cautat: ");
	afisareAngajat(getAngajatById(radacina, 5));

	printf("\nNumar noduri: %d\n", determinaNrNoduri(radacina));
	printf("\nInaltime arbore: %d\d", calculeazaInaltimeArbore(radacina));
	printf("\nTotal salarii: %.2f\n", calculeazaTotalSalarii(radacina));
	printf("\nSalariul total al unui angajat cu acelasi nume este: %.2f\n",
		calculeazaSalariulTotalAlUnuiAngajat(radacina, "Andrei"));

	dezalocareArboreDeAngajati(&radacina);

	return 0;
}
