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
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
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
					if (strcmp(client, aux->info.client) == 0) {
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

void dezalocareLS(NodLS* lista) {
	NodLS* aux = lista;
	while (aux) {
		free(aux->info.client);
		NodLS* temp = aux->next;
		free(aux);
		aux = temp;
	}
}

void dezalocareHT(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		NodLS* aux = tabela.vector[i];
		while (aux) {
			free(aux->info.client);
			NodLS* temp = aux;
			aux = aux->next;
			free(temp);
		}
	}
	free(tabela.vector);


}


int main() {

	HashTable tabela;
	tabela.dimensiune = 70;
	tabela.vector = (NodLS**)calloc(tabela.dimensiune, sizeof(NodLS*));
	char buffer[60];
	char delimitator[] = ",";
	char* token;
	FILE* f = fopen("fisier.txt", "r");
	if (!f) {
		printf("Eroare la deschiderea fisierulu.\n");

		return 1;
	}
	while (fgets(buffer, sizeof(buffer), f)) {
		Comanda c;
		token = strtok(buffer, delimitator);
		if (token) {
			c.id = atoi(token);
		}
		token = strtok(NULL, delimitator);
		if (token) {
			c.client = (char*)malloc(strlen(token) + 1);
			strcpy(c.client, token);
		}
		token = strtok(NULL, delimitator);
		if (token) {
			c.suma = atof(token);
		}
		inserareHT(tabela, c);
		free(c.client);
	}
	fclose(f);
	afisareHT(tabela);
	int numar = numarComenzi(tabela, "17/03/2026");
	printf("\n%d\n", numar);
	NodLS* listaNoua = salveazaListaNoua(tabela, "17/03/2026");
	printf("\nLista noua: \n");
	afisareLS(listaNoua);
	stergeComenzi(tabela, "17/03/2026");
	afisareHT(tabela);
	afisareLS(listaNoua);
	dezalocareHT(tabela);


	return 0;
}