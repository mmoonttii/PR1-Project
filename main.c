// Nome: Lorenzo Montalto (l.montalto@studenti.unica.it)
// Matricola: 60/61/66424
// Tipologia progetto: medio

#include "carteOstacolo.h"
#include "general.h"
#include "giocatori.h"
#include "mazzoCfu.h"
#include "memoria.h"
#include "turno.h"
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
		 checkDOPPIOE = false; // Condizione di applicazione effetto DOPPIOE

	Turno turno = {}; // Struttura turno

	char saveName[STR_LEN + EXTENSION_LEN + 1]; // Nome del salvataggio

	// ========== STARTING =============================================================================================
	fLog  = openFile(FILE_LOG, WRITE);

	printf("\nBenvenuto su Happy Little Students\n");

	// Inizio del gioco, caricamento partita o inizializzazione mazzi e giocatori
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
		for (int i = 0; i < nPlayers; ++i) {
			printGiocatore(pPlayer);   // Stampa statistiche giocatore senza mano delle carte

			// Menu delle azioni player
			do {
				printf("\nAzioni:\n"
	            "\t[%d] Gioca una carta Cfu\n"
	            "\t[%d] Controlla lo stato di altri giocatori (Cfu e carte ostacolo\n"
	            "\t[%d] Esci dalla partita\n",
		        GIOCA_CARTA, INFO_GIOCATORI, LEAVE_GAME);

				input = acquisisciInputInt(LEAVE_GAME, INFO_GIOCATORI);
				switch (input) {
					case GIOCA_CARTA:
						giocaCarta(&turno, pPlayer, &mazzoCfu, &mazzoScarti, fLog, !SPAREGGIO);
						leave = true;
						break;
					case INFO_GIOCATORI:
						infoGiocatori(playerList, pPlayer, nPlayers);
						leave = false;
						break;
					case LEAVE_GAME:
						fclose(fLog);
						fclose(fSave);
						end(playerList, mazzoCfu, mazzoScarti, mazzoOstacoli, &turno);
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
		printPunti(&turno, playerList, nPlayers);

		minMax(turno.points, nPlayers, &turno.cfuToLose, &turno.cfuToWin);

		gestioneEffetti(nPlayers, playerList, &mazzoCfu, &mazzoScarti, &turno, &checkDOPPIOE);
		printPunti(&turno, playerList, nPlayers);

		gestioneInstantPunteggio(nPlayers, playerList, fLog, &turno, checkDOPPIOE);
		printPunti(&turno, playerList, nPlayers);


		winnersLosers(&turno, playerList, nPlayers);

		printLosers(turno.losers);
		printWinners(turno.winners);

		// ==== GESTIONE PERDENTI / SPAREGGI ===========
		/* Se non ci sono vincitori o perdenti, dopo aver eseguito winnersLosers(), vuol dire che tutti i giocatori
		 * hanno pareggiato e la carta ostacolo viene rimessa in fondo al mazzo
		 */

		// Se non ci sono giocatori perdenti o vincenti (quindi cfuToWin == cfuToLose), la carta va in fondo al mazzo
		if (turno.cfuToLose == turno.cfuToWin) {
			printf("\nTutti i giocatori sono a parimerito\n"
				   "La carta ostacolo di questo turno verrà messa alla fine del mazzo\n");
			ostacoloInCoda(turno.cartaOstacolo, &mazzoOstacoli);
		} else {
			assegnaPunti(&turno, playerList, nPlayers);   // Assegnazione punti ai vincitori

			losersCount = contaLosers(&turno); // Conta i giocatori che hanno perso

			// Assegno a pLoser il giocatore perdente
			if (losersCount == 1) {
				// Se è solo uno, direttamente turno.losers
				pLoser = turno.losers;
			} else {
				// Altrimenti devo risolvere gli spareggi
				printf("\nRisoluzione spareggi");
				pLoser = gestisciSpareggi(losersCount, &turno, &mazzoScarti, &mazzoCfu, fLog);
			}
			// Assegno la carta ostacolo al giocatore perdente
			ostacoloALoser(&turno, playerList, pLoser);
		}

		// ==== FINE DEL TURNO ==========
		// Aggiugno al mazzo Scarti le carte giocate questo turno
		mazzoScarti = scartaCarte(&turno.carteGiocate, mazzoScarti);

		// Assegno i punti per le carte ostacolo in mano
		puntiCarteOstacolo(playerList);

		// Controllo che la partita non sia finita
		endGame = playerCheck(&nPlayers, &playerList, &mazzoOstacoli, &mazzoScarti);

		// Se la partita non è finita
		if (!endGame) {
			printf("\nDistribuendo le nuove carte...\n");
			pPlayer = playerList;
			// Distribuisco le carte a tutti i giocatori ancora in giuoco fino ad averne 5 in mano
			for (int i = 0; i < nPlayers; ++i) {
				distribuisciCarte(pPlayer->manoCarteCfu, &mazzoCfu, &mazzoScarti);
				pPlayer = pPlayer->nextPlayer;
			}
		}

		// Pulisco le variabili di ogni turno
		turno.winners = freeGiocatore(turno.winners);
		turno.losers = freeGiocatore(turno.losers);
		turno.points = freeIntArr(turno.points);
		turno.numTurno++;
		turno.cartaOstacolo = NULL;
		pLoser = NULL;
	}

	// A fine partita chiudo il file di log e pulisco la memoria
	fclose(fLog);
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	return 0;
}
