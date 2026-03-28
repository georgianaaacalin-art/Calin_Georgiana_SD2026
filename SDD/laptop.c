#include <stdio.h>
#include <stdlib.h>

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

void afisare(struct Laptop lap) {
	if (lap.producator != NULL) {
		printf("%d Laptopul %s are %d GB si costa %.2f\n", lap.id, lap.producator, lap.RAM, lap.pret);
	}
	else {
		printf("%d Laptopul are %d GB si costa %.2f\n", lap.id, lap.RAM, lap.pret);
	}
}

void modificaPret(struct Laptop* lap, float noulPret) {
	if (noulPret > 0) {
		lap->pret = noulPret;
	}
}

void dezalocare(struct Laptop* lap) {
	if (lap->producator != NULL) {
		free(lap->producator);
		lap->producator = NULL;
	}
}

int main() {
	struct Laptop lap;
	lap = initializare(1, 300, "Lenovo", 5000.5);
	afisare(lap);
	modificaPret(&lap, 1000);

	afisare(lap);
	dezalocare(&lap);
	afisare(lap);

	return 0;
}

