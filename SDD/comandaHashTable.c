#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//am declarat structura Comanda + alias
typedef struct Comanda Comanda;
struct Comanda {
	unsigned int id;
	char* client;
	float suma;
};

//am declarat structura NodLS + alias
typedef struct NodLS NodLS;
struct NodLS {
	struct Comanda info;
	struct NodLS* next;
};

//am declarat structura HashTable + alias
typedef struct HashTable HashTable;
struct HashTable {
	int dimensiune;
	NodLS** vector;
};

Comanda creareComanda(unsigned int id, const char* client, float suma) {
	struct Comanda c;
	c.id = id;
	c.client = (char*)malloc(strlen(client) + 1);
	strcpy(c.client, client);
	c.suma = suma;

	return c;
}

int functieInserare(HashTable tabela, const char* client) {
	return client[0] % tabela.dimensiune;
}

int inserareHT(HashTable tabela, Comanda c) {
	int pozitie = functieInserare(tabela, c.client);
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = creareComanda(c.id, c.client, c.suma);
	nou->next = NULL;
	if (tabela.vector[pozitie] == NULL) {
		tabela.vector[pozitie] = nou;
	}
	else {
		NodLS* aux = tabela.vector[pozitie];
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	return pozitie;
}

void afisareLS(NodLS* cap) {
	NodLS* aux = cap;
	while (aux) {
		printf("Comanda: %u, livrata la clientul %s, cu suma de %.2f\n",
			aux->info.id, aux->info.client, aux->info.suma);
		aux = aux->next;
	}
}

NodLS* inserareInceput(NodLS* cap, Comanda c) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS*));
	nou->info = creareComanda(c.id, c.client, c.suma);
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		nou->next = cap;
		cap = nou;
	}
	return cap;
}

void afisareHT(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		if (tabela.vector[i]) {
			printf("Pozitie %d: \n", i);
			NodLS* aux = tabela.vector[i];
			while (aux) {
				printf("Comanda %u a ajuns la clientul %s cu suma de %.2f\n",
					aux->info.id, aux->info.client, aux->info.suma);
				aux = aux->next;
			}
		}

	}

}

int numarComenzi(HashTable tabela, const char* client) {
	int numar = 0;
	if (tabela.vector) {
		for (int i = 0; i < tabela.dimensiune; i++) {
			if (tabela.vector[i]) {
				NodLS* aux = tabela.vector[i];
				while (aux) {
					if (strcmp(client, aux->info.client == 0)) {
						numar++;
					}
					aux = aux->next;
				}
			}
		}
		return numar;
	}
}

//functie de salvare a unei liste noi
NodLS* salveazaListaNoua(HashTable tabela, const char* client) {
	NodLS* listaNoua = NULL;
	if (tabela.vector) {
		for (int i = 0; i < tabela.dimensiune; i++) {
			if (tabela.vector[i]) {
				NodLS* aux = tabela.vector[i];
				while (aux) {
					if (strcmp(client, aux->info.client) == 0) {
						listaNoua = inserareInceput(listaNoua, aux->info);

					}
					aux = aux->next;
				}
			}
		}
		return listaNoua;
	}
}

void stergeComenzi(HashTable tabela, const char* client) {
	int numar = 0;
	if (tabela.vector) {
		for (int i = 0; i < tabela.dimensiune; i++) {
			if (tabela.vector[i]) {
				NodLS* aux = tabela.vector[i];
				NodLS* prev = NULL;
				while (aux) {
					if (strcmp(client, aux->info.client) == 0) {
						NodLS* temp = aux;
						if (prev == NULL) {
							tabela.vector[i] = aux->next;
							aux = tabela.vector[i];
						}
						else {
							prev->next = aux->next;
							aux = prev->next;
						}
						free(temp->info.client);
						free(temp);
					}
					else {
						prev = aux;
						aux = aux->next;
					}
				}
			}
		}
	}
}


int main() {

	return 0;
}