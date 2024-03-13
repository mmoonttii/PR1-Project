#include "istantanee.h"
#include "mazzoCfu.h"
#include "turno.h"
#include "saves.h"
#include "carteOstacolo.h"

// ========== ISTANTANEE PUNTEGGIO =====================================================================================
/**
 * gestioneInstantPunteggio() gestisce l'utilizzo delle carte istantanee giocabili al momento del calcolo del punteggio
 * @param nPlayers int: numero dei giocatori
 * @param playerList Player *: lista dei giocatori
 * @param fLog FILE *: puntatore file di lof
 * @param turno Turno *: struttura Turno
 * @param checkDOPPIOE bool: indicatore di attivazione effetto checkDOPPIOE
 */
void gestioneInstantPunteggio(int nPlayers, Player *playerList,
							  FILE *fLog, Turno *turno,
							  bool checkDOPPIOE) {
	int choice           = 0;
	Player *currPlayer   = NULL;
	CartaCfu *currCarte  = NULL,
			 *playedCard = NULL;
	bool stop = false;

	currPlayer = playerList;

	// Ciclo in azione fino a quando non viene fermato da una scelta dell'utente
	while (!stop) {
		currPlayer = playerList;

		// Ciclo sui partecipanti per chiedere chi vuole giocare la carta
		printf("\nChi vuole giocare una carta istantanea?\n");
		for (int i = 1; i <= nPlayers; ++i) {
			printf("[%d] %s\n", i, currPlayer->username);
			currPlayer = currPlayer->nextPlayer;
		}
		printf("[0] Nessuno vuole giocare carte istantanee\n");

		choice = acquisisciInputInt(0, nPlayers + 1);

		currPlayer = playerList;
		// La scelta 0 permette di lasciare il loop
		if (choice == 0) {
			stop = true;
		} else {
			// Diminuisco "choice" perchè nel loop di print, le scelte erano incrementate di 1
			choice--;
			// Raggiungo il choice - 1 player
			for (int i = 0; i < choice; ++i) {
				currPlayer = currPlayer->nextPlayer;
			}
			playedCard = giocaInstantPunteggio(currPlayer, fLog, turno->numTurno);
			// Se la scelta della carta punteggio è andata a buon fine posso risolverne l'effetto
			if (playedCard != NULL) {
				risolviInstantPunteggio(nPlayers, playerList, playedCard, turno, checkDOPPIOE);
			}
		}
	}
}

/**
 * giocaInstantPunteggio() permette all'utente di scegliere quale carta istantanea giocare
 * @param pPlayer Player *: puntatore al giocatore che gioca la carta
 * @param fLog FILE *: puntatore al file di log
 * @param numTurno int: numero di turno
 * @return CartaCfu *: carta scelta dal giocatore
 */
CartaCfu *giocaInstantPunteggio(Player *pPlayer, FILE *fLog, int numTurno) {
	CartaCfu *currCarte   = pPlayer->manoCarteCfu,
	         *choosenCard = NULL;
	int      cardCount    = 0,
	         choice       = 0;
	bool     atLeastOneInstant = false,
	         instantCheck      = false;

	// Controllo che il giocatore abbia almeno una carta istantanea
	while (currCarte != NULL) {
		if (isIstantaneaPunteggio(currCarte)) {
			atLeastOneInstant = true;
		}
		currCarte = currCarte->next;
	}

	if (atLeastOneInstant) {
		cardCount = contaCarteCfu(pPlayer->manoCarteCfu);
		// Stampo le carte della mano
		for (int i = 0; i < cardCount; ++i) {
			printf("[%d] ", i);
			printSingleCartaCfu(currCarte);
			if (isIstantaneaPunteggio(currCarte)) {
				printf("| Puoi giocare questa carta\n");
			}
		}

		// Acquisisco scelta dell'utente
		do {
			choice    = acquisisciInputInt(0, cardCount);
			currCarte = pPlayer->manoCarteCfu;

			// Identifico la carta scelta dal giocatore
			choosenCard = findCartaCfu(currCarte, choice);
			instantCheck = isIstantaneaPunteggio(choosenCard);

			// Lascio il loop solo se la carta selezionata è ammissibile in questa fase
			if (!instantCheck) {
				printf("\nHai selezionato una carta che non è istantanea, riprova");
			}

		} while (!instantCheck);

		// Una volta lasciato il loop posso estrarre la carta e stampare sul log
		choosenCard = estraiCartaCfu(&currCarte, choosenCard);
		logPrintLn(fLog, numTurno, pPlayer->username, choosenCard->name);
	} else {
		// Se non si hanno carte istantanee
		printf("\nNon hai carte istantanee in mano\n");
		choosenCard = NULL;
	}

	return choosenCard;
}

/**
 * isIstantaneaPunteggio() controlla che laa carta in input sia una carat istantanea punteggio o no
 * @param cartaCfu CartaCfu *: carta da analizzare
 * @return bool: risultato del controllo
 */
bool isIstantaneaPunteggio(CartaCfu *cartaCfu) {
	bool ret = false;
	// la carta è ammissimbile se ha effetto aumenta, diminuisci o inverti
	if (cartaCfu->effect >= AUMENTA && cartaCfu->effect <= INVERTI) {
		ret = true;
	}
	return ret;
}

/**
 * risolviInstantPunteggio() chiama le subroutine corrispondenti all'effetto della carta
 * @param nPlayers int: numero di giocatori
 * @param cartaCfu CartaCgju *: carta da risolvere
 * @param playerList Plater *: lista dei giocatorio
 * @param turno Turno *: struttura turno
 * @param checkDOPPIOE bool: flag effetto DOPPIOE
 */
void risolviInstantPunteggio(int nPlayers, Player *playerList,
							 CartaCfu *cartaCfu, Turno *turno,
							 bool checkDOPPIOE) {
	switch (cartaCfu->effect) {
		case AUMENTA:
			effettoAUMENTA(nPlayers, playerList, turno, checkDOPPIOE);
			minMax(turno->points, nPlayers, &(turno->cfuToLose), &(turno->cfuToWin));
			break;
		case DIMINUISCI:
			effettoDIMINUISCI(nPlayers, playerList, turno, checkDOPPIOE);
			minMax(turno->points, nPlayers, &(turno->cfuToLose), &(turno->cfuToWin));
			break;
		case INVERTI:
			effettoINVERTI(nPlayers, turno);
			break;
		default:
			printf("Errore");
			break;
	}
}

/* ---------- Effetti ----------------------------------------------------------------------------------------------- */
/**
 * effettoAUMENTA() subroutine per l'effetto AUMENTA:\n
 * 	Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta
 * @param nPlayers int: nunmero dei giocatori
 * @param playerList Player *: lista dei giocatori
 * @param turno TurnO *: struttura turno
 * @param checkDOPPIOE bool: flag attivazione effetto DOPPIOE
 */
void effettoAUMENTA(int nPlayers, Player *playerList,
					Turno *turno, bool checkDOPPIOE) {
	int choice = 0,
		mult;
	Player *currPlayer = NULL;

	currPlayer = playerList;

	// Stampo i giocatori
	printf("\nA chi vuoi aumentare il punteggio?\n");
	for (int i = 0; i < nPlayers; ++i) {
		printf("[%d] %s\n", i, currPlayer->username);
		currPlayer = currPlayer->nextPlayer;
	}
	choice = acquisisciInputInt(0, nPlayers);

	// Se checkDOPPIOE è vero, mult prende 2, altrimenti uno
	mult = checkDOPPIOE ? ON : OFF;

	// Aggiungo al punteggio del giocatore scelto 2 o 4 CFU
	turno->points[choice] += (ADDING * mult);
}

/**
 * effettoDIMINUISCI() subroutine per l'effetto DIMINUISCI:\n
 * 	Diminusci di 2 CFU il punteggio del turno di un giocatore a scelta
 * @param nPlayers int: nunmero dei giocatori
 * @param playerList Player *: lista dei giocatori
 * @param turno TurnO *: struttura turno
 * @param checkDOPPIOE bool: flag attivazione effetto DOPPIOE
 */
void effettoDIMINUISCI(int nPlayers, Player *playerList,
					   Turno *turno, bool checkDOPPIOE) {
	int choice = 0,
		mult;
	Player *currPlayer = NULL;

	currPlayer = playerList;
	// Stampo i giocatori
	printf("\nA chi vuoi diminuire il punteggio?\n");
	for (int i = 0; i < nPlayers; ++i) {
		printf("[%d] %s\n", i, currPlayer->username);
		currPlayer = currPlayer->nextPlayer;
	}

	// Ricevo l'input del gioactore, assicurandomi che sia valido
	choice = acquisisciInputInt(0, nPlayers);

	// Se checkDOPPIOE è vero, mult prende 2, altrimenti uno
	mult = checkDOPPIOE ? ON : OFF;

	// Aggiungo al punteggio del giocatore scelto 2 o 4 CFU
	turno->points[choice] -= ADDING * mult;
}

/**
 * effettoINVERTI() subroutine per l'effetto INVERTI:\n
 * 	Inverti punteggio minimo e massimo del turno
 * @param nPlayers int: numero di giocatori
 * @param turno Turno *: struttura turno
 */
void effettoINVERTI(int nPlayers, Turno *turno) {
	for (int i = 0; i < nPlayers; ++i) {
		if (turno->points[i] == turno->cfuToWin) {
			turno->points[i] = turno->cfuToLose;
		} else if (turno->points[i] == turno->cfuToLose) {
			turno->points[i] = turno->cfuToWin;
		}
	}
}

// ========== ISTANTANEE PERDITA =======================================================================================
/**
 * gestioneInstantPerdita() gestisce l'utilizzo delle carte istantanee giocabili al momento della perdita del turno
 * @param numTurno int: numero del turno in corso
 * @param pPlayer Player *: puntatore al giocatore attuale
 * @param playerList Player *: lista di fiocatori
 * @param mazzoOstacoli CartaOstacolo **: mazzo degli ostacoli
 * @param pCartaOstacolo CartaOstacolo **: puntatore alla carta ostacolo che ha fatto perdere il giocatore
 * @param fLog FILE *: puntatore al file di log
 */
void gestioneInstantPerdita(int numTurno,
							Player *pPlayer, Player *playerList,
							CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo,
							FILE *fLog) {
	char choice;
	CartaCfu *choosenCard = NULL;
	printf("\nHai perso il turno, vuoi controllare se puoi giocare una carta istantanea?");

	// Ricevo l'input dell'utente assicurando chje sia valido
	do {
		printf("\n>>> [y/n]");
		scanf(" %c", &choice);

		if (choice != 'y' || choice != 'n') {
			printf("\nInput non valido, riprovare");
		}
	} while (choice != 'y' || choice != 'n');

	switch (choice) {
		case 'y':
			choosenCard = giocaInstantPerdita(numTurno, pPlayer, fLog);
			break;
		case 'n':
			ostacoloInCoda(*pCartaOstacolo, &(pPlayer->listaCarteOstacolo));
			break;
		default:
			printf("\nErrore\n");
	}
	if (choosenCard != NULL) {
		risolviInstantPerdita(playerList, choosenCard, mazzoOstacoli, pCartaOstacolo);
	} else {
		printf("\nNon hai carte istantanee in mano, devi prendere l'ostacolo");
		ostacoloInCoda(*pCartaOstacolo, &(pPlayer->listaCarteOstacolo));
	}
}

/**
 * giocaInstantPerdita() acquisisce una carta istantanea perdita
 * @param numTurno int: numero del turno corrente
 * @param pPlayer Player *: giocatore che deve scegliere la carta
 * @param fLog FILE *: handler del file di log
 * @return CartaCfu *: carta cfu scelta dal player
 */
CartaCfu *giocaInstantPerdita(int numTurno, Player *pPlayer, FILE *fLog) {
	CartaCfu *currCarte   = pPlayer->manoCarteCfu,
	         *choosenCard = NULL;
	int      i            = 0,
	         choice       = 0;
	bool     atLeastOneInstant = false,
	         instantCheck      = false;

	// Ciclo sulle carte della mano e le stampo
	while (currCarte != NULL) {
		if (isIstantaneaPerdita(currCarte)) { // Controllo che il giocatore abbia almeno una istantanea
			atLeastOneInstant = true;
		}
		printf("[%d] ", i);
		printSingleCartaCfu(currCarte);
		currCarte = currCarte->next;
		i++;
	}

	// Se ha alm eno una carta istantanea, gli permetto di giocare una carta dalla sua mano
	if (atLeastOneInstant) {
		do {
			choice    = acquisisciInputInt(0, i);
			currCarte = pPlayer->manoCarteCfu;

			choosenCard = findCartaCfu(currCarte, choice);
			instantCheck = isIstantaneaPerdita(choosenCard);

			if (!instantCheck) {
				printf("\nHai selezionato una carta che non è istantanea, riprova");
			}

		} while (!instantCheck);

		// Scelta una carta, la estraggo dalla mano e la restituisco
		choosenCard = estraiCartaCfu(&currCarte, choosenCard);
		logPrintLn(fLog, numTurno, pPlayer->username, choosenCard->name);
	} else {
		printf("\nNon hai carte istantanee in mano\n");
		choosenCard = NULL;
	}

	return choosenCard;
}

/**
 * isIstantaneaPunteggio() controlla che laa carta in input sia una carat istantanea perdita o no
 * @param cartaCfu CartaCfu *: carta da analizzare
 * @return bool: risultato del controllo
 */
bool isIstantaneaPerdita(CartaCfu *cartaCfu) {
	bool ret = false;
	// la carta è ammissimbile se ha effetto salva o dirotta
	if (cartaCfu->effect == SALVA || cartaCfu->effect == DIROTTA) {
		ret = true;
	}
	return ret;
}

/**
 * risolviInstantPerdita() chiama le subroutine corrispondenti all'effetto della carta
 * @param playerList Plater *: lista dei giocatorio
 * @param pCartaCfu CartaCgju *: carta da risolvere
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore al mazzo carte ostacoli
 * @param pCartaOstacolo CartaOstacolo **: doppio puntatore alla carta ostacoli del turno
*/
void risolviInstantPerdita(Player *playerList, CartaCfu *pCartaCfu,
						   CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo) {
	switch (pCartaCfu->effect) {
		case SALVA:
			effettoSALVA(pCartaCfu, mazzoOstacoli, pCartaOstacolo);
			break;
		case DIROTTA:
			effettoDIROTTA(NULL, playerList, pCartaCfu, pCartaOstacolo);
			break;
		default:
			break;
	}
}

/* ---------- Effetti ----------------------------------------------------------------------------------------------- */
/**
 * effettoSALVA() è la subroutine per l'effetto SALVA:\n
 * 	Metti la carta Ostacolo che stai per prendere in fondo al mazzo
 * @param pCartaCfu
 * @param mazzoOstacoli
 * @param pCartaOstacolo
 */
void effettoSALVA(CartaCfu *pCartaCfu,
                  CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo) {
	ostacoloInCoda(*pCartaOstacolo, mazzoOstacoli);
}

/**
 * effettoDIROTTA() è la subroutine per l'effetto DIROTTA:\n
 * 	Dai la carta che stai per prendere ad un altro giocatore a tua scelta
 * @param playerList
 * @param pCartaCfu
 * @param pCartaOstacolo
 */
void effettoDIROTTA(Player *pPlayer, Player *playerList,
					CartaCfu *pCartaCfu,
					CartaOstacolo **pCartaOstacolo) {
	Player *currPlayer = NULL;
	currPlayer = playerList;
	int count  = 0,
		choice = 0;

	bool flag = false;

	// Stampa giocatori
	printf("\nA chi vuoi dare questa carta ostacolo?\n");
	while (currPlayer != NULL) {
		printf("[%d] %s", count, currPlayer->username);
		count++;
		currPlayer = currPlayer->nextPlayer;
	}

	// Acquisisco giocatore su cui dirottare
	do {
		currPlayer = playerList;
		choice = acquisisciInputInt(0, count - 1);

		// Ciclo fino al gikocatore
		for (int i = 0; i < choice; ++i) {
			currPlayer = currPlayer->nextPlayer;
		}
		// Se il giocatore è se stessi, allora chiedo di riptere l'input
		if (currPlayer == pPlayer) {
			printf("\nHai scelto te stesso, riprova\n");
			flag = true;
		}
	} while (flag);

	// Selezionato il gicoatore, posso assgnarli la carta sotacolo
	ostacoloInCoda(*pCartaOstacolo, &currPlayer->listaCarteOstacolo);
}