#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Creare structura Restaurat
typedef struct Restaurant {

    int nrAngajati;
    char* adresa;
    int nrLivrari;

} Restaurant;

// Creare structura Nod
typedef struct Nod {

    Restaurant info;
    struct Nod* stanga;
    struct Nod* dreapta;

} Nod;

// Initializare Restaurant
Restaurant initializareRestaurant(int nrAngajati,
    const char* adresa, int nrLivrari) {

    Restaurant r;
    r.nrAngajati = nrAngajati;
    r.adresa = (char*)malloc(sizeof(char) * (strlen(adresa) + 1));
    strcpy(r.adresa, adresa);
    r.nrLivrari = nrLivrari;

    return r;

}

// Creare nod in arbore
Nod* creareNod(Restaurant r, Nod* stanga, Nod* dreapta) {

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = initializareRestaurant(r.nrAngajati,
        r.adresa, r.nrLivrari);
    nou->stanga = stanga;
    nou->dreapta = dreapta;

    return nou;

}

// Inserare nod in arbore dupa cod
Nod* inserareArbore(Nod* radacina, Restaurant r) {

    if (radacina) {

        if (r.nrAngajati < radacina->info.nrAngajati) {

            radacina->stanga = inserareArbore(radacina->stanga, r);
            return radacina;

        }

        else if (r.nrAngajati > radacina->info.nrAngajati) {

            radacina->dreapta = inserareArbore(radacina->dreapta, r);
            return radacina;

        }
        else {

            return radacina;

        }

    }

    return creareNod(r, NULL, NULL);

}

// Afisare Preordine Inordine Postordine
void afisarePreordine(Nod* radacina) {

    if (radacina) {

        printf("\n%d %s% %d\n", radacina->info.nrAngajati,
            radacina->info.adresa, radacina->info.nrLivrari);
        afisarePreordine(radacina->stanga);
        afisarePreordine(radacina->dreapta);

    }

}

void afisareInordine(Nod* radacina) {

    if (radacina) {

        afisareInordine(radacina->stanga);
        printf("\n%d %s% %d\n", radacina->info.nrAngajati,
            radacina->info.adresa, radacina->info.nrLivrari);
        afisareInordine(radacina->dreapta);

    }

}

void afisarePostordine(Nod* radacina) {

    if (radacina) {

        afisarePostordine(radacina->stanga);
        afisarePostordine(radacina->dreapta);
        printf("\n%d %s% %d\n", radacina->info.nrAngajati,
            radacina->info.adresa, radacina->info.nrLivrari);

    }

}

// Recursivitate
int maxim(int a, int b) {

    return (a > b) ? a : b;

}

int nrNiveluri(Nod* radacina) {

    if (radacina) {

        return 1 + maxim(nrNiveluri(radacina->stanga), nrNiveluri(radacina->dreapta));

    }

    return 0;

}

// Dezalocare arbore binar
void dezalocareArbore(Nod* radacina) {

    if (radacina) {

        dezalocareArbore(radacina->stanga);
        dezalocareArbore(radacina->dreapta);
        free(radacina->info.adresa);
        free(radacina);

    }

}

// Contorizare restaurante
void contorizareRestaurante(Nod* radacina, int* contor) {

    if (radacina) {

        (*contor)++;
        contorizareRestaurante(radacina->stanga, contor);
        contorizareRestaurante(radacina->dreapta, contor);

    }

}

// Nr total frunze
void nrLivrariFrunze(Nod* radacina, int* suma) {

    if (radacina == NULL) {}
    return;
    if (radacina->stanga == NULL && radacina->dreapta == NULL) {

        *suma += radacina->info.nrLivrari;

    }

    nrLivrariFrunze(radacina->stanga, suma);
    nrLivrariFrunze(radacina->dreapta, suma);

}

// Afisare main
int main() {
    Restaurant r;
    Nod* radacina = NULL;

    char buffer[100];
    char separator[] = ",";
    char* token;

    FILE* f = fopen("restaurant", "r");
    if (!f) {

        printf("Fisierul nu s-a putut deschide!\n");
        return -1;

    }

    while (fgets(buffer, sizeof(buffer), f)) {

        token = strtok(buffer, separator);
        r.nrAngajati = atoi(token);

        token = strtok(NULL, separator);
        r.adresa = (char*)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(r.adresa, token);

        token = strtok(NULL, separator);
        r.nrLivrari = atoi(token);

        radacina = inserareArbore(radacina, r);
        free(r.adresa);

    }
    fclose(f);

    printf("\n =======  Afisare in preordine  ======= \n");
    afisarePreordine(radacina);

    int nr = nrNiveluri(radacina);
    printf("\nNumar de niveluri: %d\n", nr);

    int contor = 0;
    contorizareRestaurante(radacina, &contor);
    printf("Numar total de noduri: %d\n", contor);

    int suma = 0;
    nrLivrariFrunze(radacina, &suma);
    printf("Nr total frunze: %d\n", suma);

    dezalocareArbore(radacina);

    return 0;

}