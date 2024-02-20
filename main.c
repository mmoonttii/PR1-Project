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
	// ========== DICHIARAZIONI E INIT =================================================================================
	srand(time(NULL)); // Inizializzazione seed random
	setvbuf(stdout, NULL, _IONBF, 0); // DEBUG stdout

	// Puntatori ai file
	FILE *fPersonaggi = NULL, // path: FILE_PERSONAGGI
		 *fCfu        = NULL, // path: FILE_CARTE_CFU
		 *fOstacoli   = NULL; // path: FILE_CARTE_OSTACOLO

	// Personaggi e giocatori
	Character     charactersArr[N_PERSONAGGI] = {};    // Array personaggi
	Player        *playerList                 = NULL,  // Lista giocatori
				  *pPlayer                    = NULL;  // Puntatore al Player attuale
	// Carte
	CartaCfu      *mazzoCfu                   = NULL,  // Lista mazzo pesca Carte Cfu
				  *mazzoScarti                = NULL;  // Lista mazzo scarti Carte Cfu
	CartaOstacolo *mazzoOstacoli              = NULL;  // Lista mazzo Carte Ostacolo

	int nGiocatori = 0,                                // Numero giocatori partecipanti
		input;                                         // Input menu

	bool leave   = false,                              // Condizione di uscita dal menu
		 endGame = false;                              // Condizione di termine partita
	Turno turno = {};                                  // Struttura turno

	// ========== PREPARAZIONE =========================================================================================
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
	playerList = initGiocatori(nGiocatori, &mazzoCfu, charactersArr, mazzoScarti);

	// ========== TURNI ================================================================================================

	turno.numTurno = 1;

	while (endGame != true){
		pPlayer = playerList;
		enterClear();
		printf("\n========== TURNO %d ==========\n", turno.numTurno);
		turno.cartaOstacolo = pescaCartaOstacolo(&mazzoOstacoli);
		printOstacoli(turno.cartaOstacolo);

		// Svolgimento turno
		for (int i = 0; i < nGiocatori; ++i) {
			printGiocatore(pPlayer, false);   // Stampa statistiche giocatore senza mano delle carte
			do {
				input = acquisisciAzione();
				switch (input) {
					case 1:
						giocaCarta(&pPlayer->manoCarteCfu, &turno.carteGiocate, mazzoScarti, mazzoCfu);
						leave = true;
						break;
					case 2:
						infoGiocatori(playerList, pPlayer, nGiocatori);
						leave = false;
						break;
					case 0:
						leave = true, endGame = true;
						break;
					default:
						printf("\nErrore menu\n");
						exit(ERR_MENU);
				}
			} while (leave != true);

			if (pPlayer->nextPlayer != NULL){
				pPlayer = pPlayer->nextPlayer;
			} else {
				turno.numTurno ++;
			}
			enterClear();
		}

		scartaCarte(&turno.carteGiocate, &mazzoScarti);

		printf("\nDistribuendo le nuove carte...\n");
		pPlayer = playerList;
		for (int i = 0; i < nGiocatori; ++i) {
			distribuisciCarte(pPlayer->manoCarteCfu, &mazzoCfu, &mazzoScarti);
			pPlayer = pPlayer->nextPlayer;
		}

	}

	// Free mem
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	return 0;
}


