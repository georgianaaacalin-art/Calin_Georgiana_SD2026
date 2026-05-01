#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

//structura arbore binar de cautare
typedef struct Carte Carte;

struct Carte {
	char* titlu;
	int nrPagini;
	int anPublicare;
};

typedef struct Nod Nod;
struct Nod {
	Carte info;
	struct Nod* stanga;
	struct Nod* dreapta;
};

//1.structura pt retienrea informatiilor despre o carte
Carte creareCarte(const char* titlu, int nrPagini, int anPublicare) {
	Carte c;
	c.titlu = (char*)malloc(sizeof(char) * strlen(titlu) + 1);
	strcpy(c.titlu, titlu);

	c.nrPagini = nrPagini;
	c.anPublicare = anPublicare;

	return c;
}


Nod* inserareArbore(Nod* radacina, Carte c) {
	//3.Implementati functii pt: inserarea unei carti in arbore
	if (radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = creareCarte(c.titlu, c.nrPagini, c.anPublicare);
		nou->stanga = NULL;
		nou->dreapta = NULL;

		return nou;
	}

	//2.structure de arbore de tip arbore binar de cautare in care inserarea se face dupa anul publicarii
	if (c.anPublicare < radacina->info.anPublicare) {
		radacina->stanga = inserareArbore(radacina->stanga, c);
	}
	else {
		radacina->dreapta = inserareArbore(radacina->dreapta, c);
	}

	return radacina;
}

void afisareCarte(Carte c) {
	printf("Titlu: %-25s | Pagini: %-4d | An: %d\n",
		c.titlu, c.nrPagini, c.anPublicare);
}

//afisarea cartilor in preordine - RSD
void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareCarte(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

//afisarea cartilor in inordine - SRD
void afisareInOrdine(Nod* radacina) {
	if (radacina) {
		afisareInOrdine(radacina->stanga);
		afisareCarte(radacina->info);
		afisareInOrdine(radacina->dreapta);
	}
}

//afisarea cartilor in postordine - SDR
void afisarePostOrdine(Nod* radacina) {
	if (radacina) {
		afisarePostOrdine(radacina->stanga);
		afisarePostOrdine(radacina->dreapta);
		afisareCarte(radacina->info);
	}
}

//calcul inaltimea arborelui
int inaltimeArbore(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	int inaltimeSt = inaltimeArbore(radacina->stanga);
	int inaltimeDr = inaltimeArbore(radacina->dreapta);

	return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
}

//nr total de carti
void nrCarti(Nod* radacina, int* contor) {
	if (radacina) {
		(*contor)++; //cast la pointer
		nrCarti(radacina->stanga, contor);
		nrCarti(radacina->dreapta, contor);

	}
}

//suma totala a paginilor pt cartile aflate in frunzele arborelui
void sumaPaginiFrunze(Nod* radacina, int* suma) {
	if (radacina) {
		if (radacina->stanga == NULL && radacina->dreapta == NULL) {
			(*suma) += radacina->info.nrPagini;
		}
		sumaPaginiFrunze(radacina->stanga, suma);
		sumaPaginiFrunze(radacina->dreapta, suma);
	}
}

//eliberati memoria alocata dinamic
void dezalocareArbore(Nod* radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);

		free(radacina->info.titlu);
		free(radacina);
	}
}

int main() {

	Nod* radacina = NULL;
	Carte c;
	char buffer[100];
	char* token;
	FILE* f = fopen("carti.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis\n");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), f)) {
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strlen(buffer) == 0) continue;

		token = strtok(buffer, ",");
		if (token == NULL) continue;

		char titlu[100];
		strncpy(titlu, token, sizeof(titlu) - 1);
		titlu[sizeof(titlu) - 1] = '\0';

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		int nrPagini = atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		int anPublicare = atoi(token);

		c = creareCarte(titlu, nrPagini, anPublicare);
		radacina = inserareArbore(radacina, c);

		free(c.titlu);
	}

	fclose(f);

	printf("Afisare PreOrdine: \n"); 
	afisarePreOrdine(radacina);

	printf("\nAfisare InOrdine: \n");  
	afisareInOrdine(radacina);

	printf("\nAfisare PostOrdine: \n"); 
	afisarePostOrdine(radacina);

	int inaltime = inaltimeArbore(radacina); 
	printf("\nInaltime arbore: %d\n", inaltime); 

	int contor = 0; 
	nrCarti(radacina, &contor); 
	printf("\nNr total carti: %d\n", contor); 

	
	int suma = 0;
	sumaPaginiFrunze(radacina, &suma);
	printf("Suma totala a paginilor: %d\n", suma);

	dezalocareArbore(radacina);



	return 0;
}





