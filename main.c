// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "general.h"
#include "carteOstacolo.h"
#include "carteCfu.h"
#include "turno.h"
#include "giocatori.h"

#define FILE_PERSONAGGI "../files-input/personaggi.txt"
#define FILE_CARTE_CFU "../files-input/carte.txt"
#define FILE_CARTE_OSTACOLO "../files-input/ostacoli.txt"


int main() {
	// Inizializzazione seed random
	srand(time(NULL));
	setvbuf(stdout, NULL, _IONBF, 0);

	// ========== VARIABILI ==========
	// Puntatori ai file
	FILE *fPersonaggi = NULL,
		 *fCfu        = NULL,
		 *fOstacoli   = NULL;

	// Personaggi e giocatori
	Character     charactersArr[N_PERSONAGGI] = {};    // Array strutture personaggio
	Player        *playerList                 = NULL,  // Lista giocatori
				  *pPlayer                    = NULL;  // Puntatore al Player attuale
	// Carte
	CartaCfu      *mazzoCfu                   = NULL,  // Lista mazzo pesca Carte Cfu
				  *mazzoScarti                = NULL;  // Lista mazzo scarti Carte Cfu
	CartaOstacolo *mazzoOstacoli              = NULL;  // Lista mazzo Carte Ostacolo

	int nGiocatori = 0,                             // Numero giocatori partecipanti
		input;

	Turno turno = {};

	// ========== PREPARAZIONE ================================================
	// Apertura file
	fPersonaggi = openFile(FILE_PERSONAGGI, READ);
	fCfu        = openFile(FILE_CARTE_CFU, READ);
	fOstacoli   = openFile(FILE_CARTE_OSTACOLO, READ);

	// Creazione array charactersArr
	parseCharacters(fPersonaggi, charactersArr);

	// Lettura carte e creazione mazzo mischiato
	mazzoCfu      = creaMazzoCfu(fCfu);
	mazzoOstacoli = creaMazzoOstacoli(fOstacoli);

	// Chiusura file
	fclose(fPersonaggi);
	fclose(fOstacoli);
	fclose(fCfu);

	// Giocatori
	nGiocatori = acquisisciNumGiocatori();
	playerList = initGiocatori(nGiocatori, &mazzoCfu, charactersArr);

	// ========== TURNO =======================================================

	turno.numTurno = 1;

	//while (endGame != true){
		pPlayer   = playerList;
		printf("\n========== TURNO %d ==========\n", turno.numTurno);
		turno.cartaOstacolo = pescaCartaOstacolo(&mazzoOstacoli);
		printOstacoli(turno.cartaOstacolo);

	for (int i = 0; i < nGiocatori; ++i) {
		printGiocatore(pPlayer, false);   // Stampa statistiche giocatore senza mano delle carte

		input = acquisisciAzione();
		switch (input) {
			case 1:
				giocaCarta(&pPlayer->manoCarteCfu, turno.carteGiocate);
				break;
			case 2:
				infoGiocatori(playerList);
				break;
			case 0:
				end();
		}
	}


	//}

	// Free mem
	mazzoCfu = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	return 0;
}


