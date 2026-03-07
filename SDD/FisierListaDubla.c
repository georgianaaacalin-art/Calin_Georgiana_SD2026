#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Fisier alias;
struct Fisier {

    unsigned int id;
    char* numeF;
    int memorie;


};

alias creareFisier(unsigned int id, const char* numeF, int memorie) {
    alias a;
    a.id = id;
    a.numeF = (char*)malloc(sizeof(char) * strlen(numeF) + 1);
    strcpy(a.numeF, numeF);
    a.memorie = memorie;

    return a;
}

//primul nod este structura, al doilea nod este aliasul
typedef struct Nod nod;
struct Nod {
    struct Nod* prev;
    alias info;
    struct Nod* next;
};

//structura o scriem cu litera mare, aliasurile cu litera mica
typedef struct LD ld;
struct LD {
    nod* prim;
    nod* ultim;
};

void inserareInceput(ld* lista, alias a) {

    nod* nou = (nod*)malloc(sizeof(nod));
    nou->prev = NULL;
    nou->info = creareFisier(a.id, a.numeF, a.memorie);
    nou->next = NULL;

    if (lista->prim == NULL) {

        lista->prim = nou;
        lista->ultim = nou;
    }
    else {

        nou->next = lista->prim;
        lista->prim->prev = nou;
        lista->prim = nou;
    }
}

ld inserareSfarsit(ld lista, alias a) {

    nod* nou = (nod*)malloc(sizeof(nod));
    nou->prev = NULL;
    nou->info = creareFisier(a.id, a.numeF, a.memorie);
    nou->next = NULL;

    if (lista.prim == NULL) {

        lista.prim = nou;
        lista.ultim = nou;
    }
    else {
        lista.ultim->next = nou;
        nou->prev = lista.ultim;
        lista.ultim = nou;
    }
    return lista;
}

void afisareInceput(ld lista) {
    if (lista.prim) {
        nod* aux = lista.prim;
        while (aux) {
            printf("id: %u \n", aux->info.id);
            printf("Nume fisier: %s \n", aux->info.numeF);
            printf("Memorie: %d \n", aux->info.memorie);
            printf("\n\n");
            aux = aux->next;
        }
    }
    else {
        printf("Lista este goala");
    }
}

void afisareSfarsit(ld lista) {
    if (lista.ultim) {
        nod* aux = lista.ultim;
        while (aux) {
            printf("id: %u \n", aux->info.id);
            printf("Nume fisier: %s \n", aux->info.numeF);
            printf("Memorie: %d \n", aux->info.memorie);
            printf("\n\n");
            aux = aux->prev;
        }
    }
}

void dezalocare(ld lista) {
    if (lista.prim) {
        nod* aux = lista.prim;
        while (aux) {
            free(aux->info.numeF);
            nod* temp = aux->next;
            free(aux);
            aux = temp;
        }
    }
}

int main() {

    int nr;
    alias a;
    ld lista;
    lista.prim = NULL;
    lista.ultim = NULL;
    char buffer[40];
    FILE* f = fopen("fisier.txt", "r");
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++) {
        fscanf(f, "%u", &a.id);
        fscanf(f, "%s", buffer);
        a.numeF = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
        strcpy(a.numeF, buffer);
        fscanf(f, "%d", &a.memorie);
        lista = inserareSfarsit(lista, a);
        free(a.numeF);
    }

    fclose(f);
    afisareInceput(lista);
    afisareSfarsit(lista);
    dezalocare(lista);

    return 0;
}

