#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Student Student;
struct Student {
	int nrMatricol;
	char* nume;
	float medie;
};

typedef struct Nod Nod;
struct Nod {
	Student info;
	struct Nod* stanga;
	struct Nod* dreapta;
};


Student citireStudentDinFisier(FILE* file) {
	char buffer[100]; 
	char separator[3] = ",\n"; 
	fgets(buffer, 100, file); 

	char* aux;
	Student s1;

	aux = strtok(buffer, separator);
	
	s1.nrMatricol = atoi(aux); 

	aux = strtok(NULL, separator);
	s1.nume = malloc(strlen(aux) + 1); 
	strcpy_s(s1.nume, strlen(aux) + 1, aux); 

	s1.medie = atof(strtok(NULL, separator)); 

	return s1;

}

void afisareStudent(Student student) {
	printf("\nNumar matricol: %d", student.nrMatricol);
	printf("\nNume student: %s", student.nume);
	printf("\nMedie: %.2f\n", student.medie);
}


void adaugaStudentInArbore(Nod** radacina, Student studentNou) {
	if (*radacina == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = studentNou;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.nrMatricol > studentNou.nrMatricol) {
			adaugaStudentInArbore(&((*radacina)->stanga), studentNou);

		}
		if ((*radacina)->info.nrMatricol < studentNou.nrMatricol) {
			adaugaStudentInArbore(&((*radacina)->dreapta), studentNou);
		}
	}
}


Nod* citireArboreDeStudentiDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Student s = citireStudentDinFisier(f);
			adaugaStudentInArbore(&radacina, s);
		}
	}
	fclose(f);
	return radacina;
}

void afisareStudentiDinArbore(Nod* radacina) {
	if (radacina) {
		afisareStudentiDinArbore(radacina->stanga);
		afisareStudent(radacina->info);
		afisareStudentiDinArbore(radacina->dreapta);
	}
}

void afisarePreOrdine(Nod* radacina) {
	if (radacina) {
		afisareStudent(radacina->info);
		afisarePreOrdine(radacina->stanga);
		afisarePreOrdine(radacina->dreapta);
	}
}

void dezalocareArboreDeStudenti(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeStudenti(&(*radacina)->stanga);
		dezalocareArboreDeStudenti(&(*radacina)->dreapta);
		free((*radacina)->info.nume);
		free(*radacina);
		*radacina = NULL;
	}
}

Student getStudentByNrMatricol(Nod* radacina, int nrMatricol) {
	Student s; 
	s.nrMatricol = -1;

	if (radacina) {
		if (radacina->info.nrMatricol == nrMatricol) {
			s = radacina->info;
			s.nume = malloc(sizeof(char) * (strlen(radacina->info.nume) + 1));
			strcpy(s.nume, radacina->info.nume);
		}
		if (nrMatricol < radacina->info.nrMatricol) {
			s = getStudentByNrMatricol(radacina->stanga, nrMatricol);
		}
		if (nrMatricol > radacina->info.nrMatricol) {
			s = getStudentByNrMatricol(radacina->dreapta, nrMatricol);
		}
	}
	return s;

}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta) + 1;
	}
	return 0;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		return maxim(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta)) + 1;
	}
	return 0;
}

float calculeazaSumaMedii(Nod* radacina) {
	if (radacina) {
		return radacina->info.medie + calculeazaSumaMedii(radacina->stanga) +
			calculeazaSumaMedii(radacina->dreapta);

	}
	return 0;
}

float calculeazaSumaMediilorUnuiStudent(Nod* radacina, const char* nume) {
	if (radacina) { 
		
		float suma = calculeazaSumaMediilorUnuiStudent(radacina->stanga, nume) +
			calculeazaSumaMediilorUnuiStudent(radacina->dreapta, nume);

		if (strcmp(radacina->info.nume, nume) == 0) {

			suma += radacina->info.medie; 
		}
		return suma; 
	}
	return 0; 
}

int main() {
	Nod* radacina = citireArboreDeStudentiDinFisier("studenti.txt");
	afisarePreOrdine(radacina);

	printf("\nStudentul cautat: ");
	afisareStudent(getStudentByNrMatricol(radacina, 123));

	printf("\nNumar noduri: %d\n", determinaNumarNoduri(radacina));
	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("\nSuma mediilor este: %.2f\n", calculeazaSumaMedii(radacina));
	printf("\nSuma mediilor unui student este: %.2f\n", calculeazaSumaMediilorUnuiStudent(radacina, "Alexandru"));

	dezalocareArboreDeStudenti(&radacina);


	return 0;
}
