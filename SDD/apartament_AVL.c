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
	struct Nod* dreapta;
	struct Nod* stanga;
};

int calculeazaInaltimeArbore(Nod* radacina);

//afisare apartament
void afisareApartament(Apartament apartament) {
	printf("\nCodul este: %d", apartament.cod);
	printf("\nProprietarul este: %s", apartament.proprietar);
	printf("\nAre %d camere.", apartament.camere);
	printf("\nPretul este: %.2f\n", apartament.pret);
}

//initializare apartament
Apartament initializareApartament(int cod, const char* proprietar, int camere, float pret) {
	Apartament a;
	a.cod = cod;
	a.proprietar = (char*)malloc(strlen(proprietar) + 1);
	strcpy(a.proprietar, proprietar);
	a.camere = camere;
	a.pret = pret;

	return a;
}

//diferenta de inaltime
int calculDiferentaInaltimi(Nod* radacina) {
	if (radacina != NULL) {
		//inaltime stanga - inaltime dreapta
		return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
	}
	else {
		return 0;
	}
}

//rotire la stanga
void rotireLaStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

//rotire la dreapta
void rotireLaDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

//adaugare in AVL
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
			//inserare in stanga
			adaugaApartamentInArbore(&((*radacina)->stanga), apartamentNou);
		}
		if ((*radacina)->info.cod < apartamentNou.cod) {
			//inserare in dreapta
			adaugaApartamentInArbore(&((*radacina)->dreapta), apartamentNou);
		}
	}

	int diferentaInaltimi = calculDiferentaInaltimi(*radacina); //verifica dezechilibrul

	if (diferentaInaltimi == 2) { //dezechilibrat in stanga
		if (calculDiferentaInaltimi((*radacina)->stanga) == -1) {
			rotireLaStanga(&(*radacina)->stanga); //caz LR
		}
		rotireLaDreapta(radacina); //caz LL
	}

	if (diferentaInaltimi == -2) { //dezechilibrat dreapta
		if (calculDiferentaInaltimi((*radacina)->dreapta) == 1) {
			rotireLaDreapta(&(*radacina)->dreapta);
		}
		rotireLaStanga(radacina); //caz RR
	}
}
//afisare Inordine
void afisareApartamenteDinArbore(Nod* radacina) {
	if (radacina) {
		afisareApartamenteDinArbore(radacina->stanga); //stanga
		afisareApartament(radacina->info);    //radacina
		afisareApartamenteDinArbore(radacina->dreapta);  //dreapta
	}
}

//afisare Preordine
void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareApartament(radacina->info); //radacina
		afisarePreOrdine(radacina->stanga); //stanga
		afisarePreOrdine(radacina->dreapta); //dreapta
	}
}

//dezalocare arbore
void dezalocareArboreDeApartamente(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeApartamente(&(*radacina)->stanga);
		dezalocareArboreDeApartamente(&(*radacina)->dreapta);
		free((*radacina)->info.proprietar);
		free(*radacina);
		*radacina = NULL;
	}
}

//cautare in arbore dupa Cod
Apartament getApartamentByCod(Nod* radacina, int cod) {
	Apartament a; //declaram un apartament pe care il vom returna
	a.cod = -1; //valoare default: daca nu gasim apartamentul , codul ramane -1
	if (radacina) { //verificam daca nodul curent exista
		if (radacina->info.cod == cod) { //daca codul din nod este egal cu codul cautat
			a = radacina->info; //copiem toata strcutura apartamentului. Atentie: se copiaza si pointerii
			a.proprietar = malloc(sizeof(char) * strlen(radacina->info.proprietar) + 1); //alocam memorie separata pt prop
			strcpy(a.proprietar, radacina->info.proprietar); //copiem textul propritarului
		}
		if (cod < radacina->info.cod) { //daca codul cautat e mai mic decat codul curent
			a = getApartamentByCod(radacina->stanga, cod); // căutăm în subarborele stâng

		}
		if (cod > radacina->info.cod) { // dacă id-ul căutat e mai mare decât id-ul curent
			a = getApartamentByCod(radacina->dreapta, cod); // căutăm în subarborele drept
		}

	}
	return a;

}

//numar noduri
int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return determinaNumarNoduri(radacina->stanga) + //noduri stanga
			+determinaNumarNoduri(radacina->stanga) +  //noduri dreapta
			+1;            //nod curent
	}
	return 0; //arbore gol
}

//maxim
int maxim(int a, int b) {
	return (a > b ? a : b);  //returneaza valoarea mai mare
}

//inaltime arbore
int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		return maxim(calculeazaInaltimeArbore(radacina->stanga), //inaltime stanga
			calculeazaInaltimeArbore(radacina->dreapta))  //inaltime dreapta
			+ 1;   // + nod curent
	}
	return 0;
}

//pret total
float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + //pret curent
			+calculeazaPretTotal(radacina->stanga) + //stanga
			+calculeazaPretTotal(radacina->dreapta);  //dreapta
	}
	return 0;
}

float calculeazaPretulApartamentelorUnuiProprietar(Nod* radacina, const char* proprietar) {
	if (radacina) {
		float suma = calculeazaPretulApartamentelorUnuiProprietar(radacina->stanga, proprietar) +
			+calculeazaPretulApartamentelorUnuiProprietar(radacina->dreapta, proprietar);

		if (strcmp(radacina->info.proprietar, proprietar) == 0) {
			suma += radacina->info.pret; //adauga daca soferul coincide
		}
		return suma;
	}
	return 0;
}



int main() {
	Nod* radacina = NULL; //initial arborele este gol
	adaugaApartamentInArbore(&radacina, initializareApartament(101, "Alexandru Ionescu", 3, 85000));
	adaugaApartamentInArbore(&radacina, initializareApartament(200, "George Georgescu", 2, 65000));
	adaugaApartamentInArbore(&radacina, initializareApartament(202, "Andrei Andreescu", 4, 101000));
	adaugaApartamentInArbore(&radacina, initializareApartament(102, "Ilie Iliescu", 5, 120000));
	adaugaApartamentInArbore(&radacina, initializareApartament(205, "Popa Popescu", 6, 150000));
	afisarePreOrdine(radacina);

	printf("\nApartamentul cautat: ");
	afisareApartament(getApartamentByCod(radacina, 205));

	printf("\nnumar noduri: %d\n", determinaNumarNoduri(radacina));

	printf("\nInaltime arbore: %d", calculeazaInaltimeArbore(radacina));

	printf("\nPret total: %.2f\n", calculeazaPretTotal(radacina));

	printf("\nSuma preturilor apartamentelor unui proprietar : %.2f", calculeazaPretulApartamentelorUnuiProprietar(radacina, "Alexandru Ionescu"));

	dezalocareArboreDeApartamente(&radacina);


	return 0;
}