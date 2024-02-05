// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "general.h"
#include "preparazione.h"
#include "memManagement.h"

#define FILE_PERSONAGGI "../files-input/personaggi.txt"
#define FILE_CARTE_CFU "../files-input/carte.txt"
#define FILE_CARTE_OSTACOLO "../files-input/ostacoli.txt"



int main() {
	// Inizializzazione seed random
	srand(time(NULL));

	// Puntatori ai file
	FILE *fPersonaggi = NULL;
	FILE *fCfu        = NULL;
	FILE *fOstacoli   = NULL;

	// Mazzi delle carte
	CartaCfu      *mazzoCfu      = NULL;
	CartaOstacolo *mazzoOstacoli = NULL;

	// Apertura file
	fPersonaggi = openFile(FILE_PERSONAGGI, READ);
	fCfu        = openFile(FILE_CARTE_CFU, READ);
	fOstacoli   = openFile(FILE_CARTE_OSTACOLO, READ);

	// Creazione array personaggi
	Personaggio personaggi[N_PERSONAGGI] = {};
	parseCharacters(fPersonaggi, personaggi);

	// Lettura carte e creazione mazzo mischiato
	mazzoCfu      = creaMazzoCfu(fCfu);
	mazzoOstacoli = creaMazzoOstacoli(fOstacoli);

	// Giocatori
	int nGiocatori = acquisisciNumGiocatori();
	Giocatore *listaGiocatori = NULL;
	listaGiocatori = initGiocatori(nGiocatori, &mazzoCfu);

	// Chiusura file e free mem
	fclose(fPersonaggi);
	fclose(fOstacoli);
	fclose(fCfu);
	mazzoCfu = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	listaGiocatori = freeGiocatore(listaGiocatori);
	return 0;
}


