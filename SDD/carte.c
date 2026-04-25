#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <string.h>

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
//tema: metoda care sa faca verificarea cu null pt radacina
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

void dezalocareArbore(Nod* radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);

		free(radacina->info.titlu);
		free(radacina);
	}
}

int main() {

	return 0;
}





