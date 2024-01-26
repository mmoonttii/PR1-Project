// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "general.h"
#define FILE_PERSONAGGI "../files-input/personaggi.txt"



int main() {
	// ============ PERSONAGGI ================================================
	// Apertura file personaggi
	FILE *fPersonaggi = NULL;
	fPersonaggi = fopen(FILE_PERSONAGGI, "r");
	if (fPersonaggi == NULL) exit(ERR_OPEN_FILE_CHARACTERS);

	// ============ CARTE CFU =================================================





	return 0;
}
