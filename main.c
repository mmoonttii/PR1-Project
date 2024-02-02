// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "general.h"
#include "preparazione.h"

#define FILE_PERSONAGGI "../files-input/personaggi.txt"



int main() {
	// Inizializzazione seed random
	srand(time(NULL));
	// ============ PERSONAGGI ================================================
	// Apertura file personaggi
	FILE *fPersonaggi = NULL;
	fPersonaggi = openCharacters(FILE_PERSONAGGI);

	Personaggio personaggi[N_PERSONAGGI] = {};
	parseCharacters(fPersonaggi, personaggi);

	// ============ CARTE CFU =================================================





	return 0;
}
