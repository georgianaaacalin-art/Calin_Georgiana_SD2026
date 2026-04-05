#include <stdio.h>
#include <malloc.h>


struct Laptop {
	int id;
	int RAM;
	char* producator;
	float pret;
};

struct Laptop initializare(int id, int RAM, const char* producator, float pret) {
	struct Laptop lap;
	lap.id = id;
	lap.RAM = RAM;
	lap.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy_s(lap.producator, strlen(producator) + 1, producator);
	lap.pret = pret;

	return lap;
}

void afisareLaptop(struct Laptop lap) {
	if (lap.producator != NULL) {
		printf("%d. Laptopul %s are %d GB si costa %.2f\n", lap.id, lap.producator, lap.RAM, lap.pret);
	}
	else {
		printf("%d. Laptopul are %d GB si costa %.2f\n", lap.id, lap.RAM, lap.pret);
	}
}

void afisareVector(struct Laptop* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisareLaptop(vector[i]);
	}
}

struct Laptop* copiazaPrimeleElemente(struct Laptop* vector, int nrElemente, int nrElementeCopiate) {
	struct Laptop* vectorNou = NULL;
	vectorNou = (struct Laptop*)malloc(sizeof(struct Laptop) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].producator = (char*)malloc(strlen(vector[i].producator) + 1);
		strcpy_s(vectorNou[i].producator, strlen(vector[i].producator) + 1, vector[i].producator);
	}

	return vectorNou;
}

void dezalocare(struct Laptop** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].producator != NULL) {
			free((*vector)[i].producator);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaLaptopsScumpe(struct Laptop* vector, char nrElemente, float pretMinim, struct Laptop** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*dimensiune)++;
		}
	}
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	*vectorNou = (struct Laptop*)malloc(sizeof(struct Laptop) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s((*vectorNou)[k].producator, strlen(vector[i].producator) + 1, vector[i].producator);
			k++;
		}
	}
}

struct Laptop getPrimulLaptopByProducator(struct Laptop* vector, int nrElemente, const char* producator) {
	struct Laptop lap;
	lap.producator = NULL;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].producator, producator) == 0) {
			lap = vector[i];
			lap.producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s(lap.producator, strlen(vector[i].producator) + 1, vector[i].producator);
			return lap;
		}
	}
	return lap;
}



int main() {

	struct Laptop* laptops = NULL;
	int nrLaptops = 4;
	laptops = (struct Laptops*)malloc(sizeof(struct Laptop) * nrLaptops);
	laptops[0] = initializare(1, 850, "HP", 4500.99);
	laptops[1] = initializare(2, 1200, "Samsung", 5500.99);
	laptops[2] = initializare(3, 1500, "Asus", 3500.99);
	laptops[3] = initializare(4, 1700, "Apple", 7500.99);

	afisareVector(laptops, nrLaptops);

	struct Laptop* primeleLaptops = NULL;
	int nrPrimeleLaptops = 2;

	primeleLaptops = copiazaPrimeleElemente(laptops, nrLaptops, nrPrimeleLaptops);
	printf("\n\First laptops: \n");
	afisareVector(primeleLaptops, nrPrimeleLaptops);
	dezalocare(&nrPrimeleLaptops, &nrPrimeleLaptops);
	afisareVector(primeleLaptops, nrPrimeleLaptops);

	//copiaza laptops scumpe
	struct Laptop* laptopsScumpe = NULL;
	int nrLaptopsScumpe = 0;
	copiazaLaptopsScumpe(laptops, nrLaptops, 2000, &laptopsScumpe, &nrLaptopsScumpe);
	printf("\n\nExpensive laptops: \n");
	afisareVector(laptopsScumpe, nrLaptopsScumpe);
	dezalocare(&laptopsScumpe, &nrLaptopsScumpe);

	//get primul laptop by producator
	struct Laptop laptop = getPrimulLaptopByProducator(laptops, nrLaptops, "Samsung");
	printf("\n\nLaptop found: \n");
	afisareLaptop(laptop);
	if (laptop.producator != NULL) {
		free(laptop.producator);
		laptop.producator = NULL;
	}
	dezalocare(&laptops, &nrLaptops);

	return 0;
}

