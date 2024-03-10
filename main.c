// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto: medio

/// TODO RICORDATI DI TOGLIERE DBG

#include "carteOstacolo.h"
#include "general.h"
#include "giocatori.h"
#include "mazzoCfu.h"
#include "memoria.h"
#include "turno.h"
#include "spareggi.h"
#include "effetti.h"
#include "saves.h"
#include "starting.h"
#include "istantanee.h"

int main() {
	// ========== DICHIARAZIONI E INIT =================================================================================
	srand(time(NULL)); // Inizializzazione seed random
	setvbuf(stdout, NULL, _IONBF, 0); // DEBUG stdout

	// Puntatori a file
	FILE *fSave = NULL, // File di salvataggio
		 *fLog  = NULL; // File di log

	// Personaggi e giocatori
	Character     charactersArr[N_PERSONAGGI] = {};   // Array dei personaggi
	Player        *playerList                 = NULL, // Lista dei giocatori
				  *pPlayer                    = NULL, // Puntatore al Player attuale
				  *pLoser                     = NULL; // Puntatore al Player perdente

	// Carte
	CartaCfu      *mazzoCfu      = NULL, // Lista mazzo pesca Carte Cfu
				  *mazzoScarti   = NULL; // Lista mazzo scarti Carte Cfu
	CartaOstacolo *mazzoOstacoli = NULL; // Lista mazzo Carte Ostacolo

	// Contatori
	int nPlayers = 0, // Numero giocatori partecipanti
		input,        // Input menu
		losersCount;  // Contatore giocatori perdenti

	// Flag di controllo
	bool leave        = false, // Condizione di uscita dal menu
		 endGame      = false, // Condizione di termine partita
		 spareggi,             // Condizione di risoluzione spareggi
		 checkDOPPIOE = false; // Condizione di applicazione effetto DOPPIOE

	Turno turno = {}; // Struttura turno

	char saveName[STR_LEN + EXTENSION_LEN + 1]; // Nome del salvataggio

	// ========== STARTING =============================================================================================
	fLog  = openFile(FILE_LOG, WRITE);

	printf("\nBenvenuto su Happy Little Students\n");

	// inizio frl gioco, caricamento partita o inizializzazione mazzi e giocatori
	startGame(saveName,
			  charactersArr,
			  &nPlayers, &playerList,
	          &mazzoCfu, &mazzoScarti,
	          &mazzoOstacoli);

	// ========== TURNI ================================================================================================
	turno.numTurno = 1; // Inizializzazione del numero di turno

	while (endGame != true) {
		// Salvataggio partita
		fSave = openFile(saveName, BIN_WRITE);
		saveOnFile(saveName, fSave, &nPlayers, playerList,
		           mazzoCfu, mazzoScarti,
		           mazzoOstacoli);
		fclose(fSave);

		pPlayer = playerList;
		printf("\n========== TURNO %d ==========\n", turno.numTurno);

		// Pesca e stampa della carta del turno
		turno.cartaOstacolo = pescaCartaOstacolo(&mazzoOstacoli);
		printOstacoli(turno.cartaOstacolo);

		// ==== SVOLGIMENTO TURNO ==========
		for (int i = 0; i < nPlayers && endGame != true; ++i) {
			printGiocatore(pPlayer);   // Stampa statistiche giocatore senza mano delle carte

			// Menu delle azioni player
			do {
				input = acquisisciAzione();
				switch (input) {
					case GIOCA_CARTA:
						giocaCarta(&turno, pPlayer, &mazzoScarti, &mazzoCfu, fLog, !SPAREGGIO);
						leave = true;
						break;
					case INFO_GIOCATORI:
						infoGiocatori(playerList, pPlayer, nPlayers);
						leave = false;
						break;
					case LEAVE_GAME:
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

		minMax(turno.points, nPlayers, &turno.cfuToLose, &turno.cfuToWin);

		gestioneEffetti(nPlayers, playerList, &mazzoCfu, &mazzoScarti, &turno, &checkDOPPIOE);

		gestioneInstantPunteggio(nPlayers, playerList, fLog, &turno, checkDOPPIOE);

		winnersLosers(&turno, playerList, nPlayers);
		printLosers(turno.losers);
		printWinners(turno.winners);


		// Se non ci sono vincitori o perdenti, dopo aver eseguito winnersLosers(), vuol dire che tutti i giocatori
		// hanno pareggiato e la carta ostacolo viene rimessa in fondo al mazzo
		if (turno.winners == NULL && turno.losers == NULL) {
			printf("\nTutti i giocatori sono a parimerito, la carta ostacolo di questo turno verrà messa alla fine del "
				   "mazzo\n");
			ostacoloInCoda(turno.cartaOstacolo, &mazzoOstacoli);
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
			ostacoloInCoda(turno.cartaOstacolo, &pLoser->listaCarteOstacolo);
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
	fclose(fSave);
	// Free mem
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	return 0;
}
