// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "general.h"
#include "preparazione.h"

#define FILE_PERSONAGGI "../files-input/personaggi.txt"
#define FILE_CARTE_CFU "../files-input/carte.txt"
#define FILE_CARTE_OSTACOLO "../files-input/ostacoli.txt"



int main() {
	// Inizializzazione seed random
	srand(time(NULL));

	// ============ PERSONAGGI ================================================
	// Apertura file personaggi
	FILE *fPersonaggi = NULL;
	fPersonaggi = openCharacters(FILE_PERSONAGGI);

	Personaggio personaggi[N_PERSONAGGI] = {};
	parseCharacters(fPersonaggi, personaggi);

	for (int i = 0; i < N_PERSONAGGI; ++i) {
		printf("%s: ", personaggi[i].name);
		for (int j = 0; j < N_OSTACOLI; ++j) {
			printf("%d ", personaggi[i].bonusMalus[j]);
		}
		printf("\n");
	}

	fclose(fPersonaggi);
	// ============ CARTE CFU =================================================





	return 0;
}
