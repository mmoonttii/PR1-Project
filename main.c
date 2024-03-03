// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto:

#include "carteOstacolo.h"
#include "general.h"
#include "giocatori.h"
#include "mazzoCfu.h"
#include "memoria.h"
#include "turno.h"
#include "spareggi.h"
#include "effetti.h"
#include "salvataggiLog.h"

#define FILE_PERSONAGGI "../files-input/personaggi.txt"
#define FILE_CARTE_CFU "../files-input/carte.txt"
#define FILE_CARTE_OSTACOLO "../files-input/ostacoli.txt"
#define FILE_LOG "log.txt"

int main() {
	// ========== DICHIARAZIONI E INIT =================================================================================
	srand(time(NULL)); // Inizializzazione seed random
	setvbuf(stdout, NULL, _IONBF, 0); // DEBUG stdout

	// Puntatori ai file
	FILE *fPersonaggi = NULL, // path: FILE_PERSONAGGI
		 *fCfu        = NULL, // path: FILE_CARTE_CFU
		 *fOstacoli   = NULL, // path: FILE_CARTE_OSTACOLO
		 *fLog        = NULL; // path: FILE_LOG

	// Personaggi e giocatori
	Character     charactersArr[N_PERSONAGGI] = {};    // Array personaggi
	Player        *playerList                 = NULL,  // Lista giocatori
				  *pPlayer                    = NULL,  // Puntatore al Player attuale
				  *pLoser                     = NULL;  // Puntatore al Player perdente
	// Carte
	CartaCfu      *mazzoCfu                   = NULL,  // Lista mazzo pesca Carte Cfu
				  *mazzoScarti                = NULL;  // Lista mazzo scarti Carte Cfu
	CartaOstacolo *mazzoOstacoli              = NULL;  // Lista mazzo Carte Ostacolo

	int nPlayers = 0,                                  // Numero giocatori partecipanti
		input,
		losersCount;                                         // Input menu

	bool leave   = false,                              // Condizione di uscita dal menu
		 endGame = false,                              // Condizione di termine partita
		 spareggi;
	Turno turno = {};                                  // Struttura turno

	// ========== PREPARAZIONE =========================================================================================
	// Apertura file
	fPersonaggi = openFile(FILE_PERSONAGGI, READ);
	fCfu        = openFile(FILE_CARTE_CFU, READ);
	fOstacoli   = openFile(FILE_CARTE_OSTACOLO, READ);
	fLog        = openFile(FILE_LOG, WRITE);

	// Creazione array charactersArr
	parseCharacters(fPersonaggi, charactersArr);

	// Lettura carte e creazione mazzo mischiato
	mazzoCfu      = creaMazzoCfu(fCfu);
	mazzoCfu      = mescolaMazzo(&mazzoCfu);
	mazzoOstacoli = creaMazzoOstacoli(fOstacoli);

	// Chiusura file
	fclose(fPersonaggi);
	fclose(fOstacoli);
	fclose(fCfu);

	// Giocatori
	nPlayers   = acquisisciNumGiocatori();
	playerList = initGiocatori(nPlayers, &mazzoCfu, charactersArr, mazzoScarti);

	// ========== TURNI ================================================================================================
	turno.numTurno = 1;

	while (endGame != true){
		pPlayer = playerList;
		printf("\n========== TURNO %d ==========\n", turno.numTurno);
		turno.cartaOstacolo = pescaCartaOstacolo(&mazzoOstacoli);
		printOstacoli(turno.cartaOstacolo);

		// ==== SVOLGIMENTO TURNO ==========
		for (int i = 0; i < nPlayers && endGame != true; ++i) {
			printGiocatore(pPlayer);   // Stampa statistiche giocatore senza mano delle carte
			do {
				input = acquisisciAzione();
				switch (input) {
					case 1:
						giocaCartaTurno(&turno, pPlayer, &mazzoScarti, mazzoCfu, fLog, !SPAREGGIO);
						leave = true;
						break;
					case 2:
						infoGiocatori(playerList, pPlayer, nPlayers);
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
			}
			enterClear();
		}

		// ==== CALCOLO PUNTEGGI ==========
		calcolaPunteggio(&turno, playerList, nPlayers, !SPAREGGIO);
		printPuntiParziali(&turno, playerList, nPlayers);

		gestioneEffetti(&turno, playerList, nPlayers, &mazzoCfu, &mazzoScarti);

		winnersLosers(&turno, playerList, nPlayers);
		printLosers(turno.losers);
		printWinners(turno.winners);


		// Se non ci sono vincitori o perdenti, dopo aver eseguito winnersLosers(), vuol dire che tutti i giocatori
		// hanno pareggiato e la carta ostacolo viene rimessa in fondo al mazzo
		if (turno.winners == NULL && turno.losers == NULL) {
			printf("\nTutti i giocatori sono a parimerito, la carta ostacolo di questo turno verrà messa alla fine del "
				   "mazzo\n");
			ostacoloInCoda(&turno.cartaOstacolo, &mazzoOstacoli);
		} else {
			assegnaPunti(&turno, playerList, nPlayers);   // Assegnazione punti ai vincitori

			losersCount = contaLosers(&turno, playerList); // Conta i giocatori che hanno perso

			if (losersCount == 1) {
				pPlayer = playerList;
				while (turno.losers != NULL) {
					if (strcmp(turno.losers->username, pPlayer->username) == 0) {
						pLoser = pPlayer;
					}
					turno.losers = turno.losers->nextPlayer;
				}
			} else {
				printf("\nRisoluzione spareggi");
				pLoser = gestisciSpareggi(losersCount, &turno, &mazzoScarti, &mazzoCfu, fLog);
			}
			ostacoloInCoda(&turno.cartaOstacolo, &pLoser->listaCarteOstacolo);
		}

		// ==== FINE DEL TURNO ==========
		scartaCarte(&turno.carteGiocate, &mazzoScarti);
		puntiCarteOstacolo(playerList);
		endGame = playerCheck(&playerList, &mazzoOstacoli, &mazzoScarti, &nPlayers);

		if (!endGame) {
			printf("\nDistribuendo le nuove carte...\n");
			pPlayer = playerList;
			for (int i = 0; i < nPlayers; ++i) {
				distribuisciCarte(pPlayer->manoCarteCfu, &mazzoCfu, &mazzoScarti);
				pPlayer = pPlayer->nextPlayer;
			}
		}
		turno.winners = freeGiocatore(turno.winners);
		turno.losers = freeGiocatore(turno.losers);
		turno.points = freeIntArr(turno.points);
		turno.numTurno++;
		turno.cartaOstacolo = NULL;
		pLoser = NULL;

	}

	fclose(fLog);
	// Free mem
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	return 0;
}
