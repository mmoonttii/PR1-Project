#include "turno.h"
#include "carteOstacolo.h"

// ============ TURNO - FASE 1 =========================================================================================
/**
 * Acquisisci azione che si occupa di ricevere l'input del giocatore che corrisponde a un azione da compiere
 * all'inzio del turno
 * @return
 */
int acquisisciAzione() {
	int input;
	printf("\nAzioni:\n"
	       "\t[1] Gioca una carta Cfu\n"
	       "\t[2] Controlla lo stato di altri giocatori (Cfu e carte ostacolo\n"
	       "\t[0] Esci dalla partita\n"
	       ">>> ");
	do {
		scanf("%d", &input);

		if (input > 2 || input < 0) {
			printf("Input non valido, riprovare\n"
			       ">>> ");
		}
	} while (input > 2 || input < 0);
	return input;
}

/**
 * giocaCarta() è la subroutine che si occupa di gestire l'acquisizione da parte di un giocatore della carta che
 * vuole giocare e aggiungere la carta alla lista delle carte giocate
 * @param manoCarteCfu é la lista delle carte in mano al giocatore
 * @param listaCarteGiocate è la lista delle carte che sono state giocate in questo turno
 */
void giocaCarta(CartaCfu **manoCarteCfu, CartaCfu **listaCarteGiocate, CartaCfu *mazzoScarti, CartaCfu *mazzoCarteCfu) {
	CartaCfu *headMano    = *manoCarteCfu,      // Puntatore della lista delle carte in mano
	         *headGiocate = *listaCarteGiocate, // Puntatore della lista delle carte giocate per questo turno
	         *choosenCard = NULL,              // Pointer alla carta giocata in questo turno
	         *prev        = NULL;              // Pointer alla carta precedente quella giocata
	int      choice       = 0;                 // Indice della carta scelta dal player
	bool instant,                              // Flag carta istantanea per chiedere una carta diversa
	     tutteIstantanee  = true;              // Flag mano tutta di carte istantanee per rimescolare la mano

	do {
		tutteIstantanee = true;
		for (int i = 0; i < CARTE_PER_MANO; ++i) {
			// Stampa di tutte le carte
			printf("\n[%d] ", i);
			printSingleCartaCfu(headMano);
			// Check istantanea
			if (headMano->cfu == 0 && headMano->effect != SCARTAP) {
				printf("| CARTA ISTANTANEA - non puoi giocarla in questa fase del turno\n");
			} else {
				tutteIstantanee = false;
			}
			if (headMano->next != NULL) {
				headMano = headMano->next;
			}
		}
		if (tutteIstantanee == true){
			printf("\n Tutte le carte che hai in mano sono istantanee, scarta tutta la tua mano e pesca 5 nuove carte");
			enterClear();
			scartaCarte(manoCarteCfu, &mazzoScarti);
			*manoCarteCfu = distribuisciCarte(*manoCarteCfu, &mazzoCarteCfu, &mazzoScarti);
		}
	} while (tutteIstantanee == true);

	do {
		// Acquisizione carta scelta
		headMano = *manoCarteCfu;
		printf(">>> ");
		scanf("%d", &choice);

		// Se la carta scelta non è la prima, quindi choice != 0
		for (int i = 0; i < choice; i++) {
			// Scorro fino alla prossima carta, salvando la posizione della carta precedente
			prev     = headMano;
			headMano = headMano->next;
		}

		choosenCard = headMano; // La carta scelta è in ogni caso headMano

		// Check istantanea
		if (choosenCard->cfu == 0 && choosenCard->effect != SCARTAP) {
			instant = true;
		} else {
			instant = false;
		}

		if (instant) {
			printf("\nLa carta scelta è una carta istantanea, scegliere un'altra carta\n");
		}
	} while (instant);

	// Quando la carta scelta non è istantanea posso estrarla dalla lista
	if (choice != 0) {
		prev->next = choosenCard->next; // Il next della precedente prende quello che era il next della carta presa
		choosenCard->next = NULL; // Il next della carta presa viene resettato a NULL
	} else {
		manoCarteCfu = &(choosenCard->next); // la testa della mano prende la carta successiva
		choosenCard->next = NULL;
	}

	// Se la lista delle carte giocate è vuota, allora la lista è la prima carta
	if (*listaCarteGiocate == NULL) {
		*listaCarteGiocate = choosenCard;
	} else {
		// Altrimenti si scorre fino alla prima carta il cui next è null e si aggiunge la carta in coda
		while (headGiocate->next != NULL) {
			headGiocate = headGiocate->next;
		}
		// La carta successiva della lista delle carte giocate è la carta scelta dal giocatore
		headGiocate->next = choosenCard;
		choosenCard->next = NULL;
	}
}

/**
 * infoGiocateori() è la subroutine che data la lista dei giocatori si occupa di stampare le informazioni di uno
 * @param listaGiocatori è la lista dei giocatori
 */
void infoGiocatori(Player *listaGiocatori, Player *currentPlayer, int nPlayers) {
	Player *head = listaGiocatori,
		   *choosenPlayer = listaGiocatori;
	int count = 0,
		input;

	printf("\nDi quale giocatore vuoi avere informazioni?\n");

	// Stampa dei giocatori dei quali si possono avere informazioni
	for (int i = 0; i < nPlayers; ++i) {
		if (strcmp(head->username, currentPlayer->username) != 0) {
			printf("[%d] %s\n", count, head->username);
		}
		if (head->nextPlayer != NULL) {
			head = head->nextPlayer;
		}
		count++;
	}
	// Acquisizione del giocatore del quale si vogliono avere infomrmazioni
	do {
		printf("\n>>> ");
		scanf("%d", &input);

		if (input < 0 || input > count){
			printf("Valore inserito non valido, riprovare.\n");
		}
	} while (input < 0 || input > count);

	// Scorro fino al giocatore che devo stampare
	for (int i = 0; i < input; ++i) {
		if (choosenPlayer->nextPlayer != NULL){
			choosenPlayer = choosenPlayer->nextPlayer;
		}
	}
	// Stampa giocatore richiesto
	printGiocatore(choosenPlayer, false);
}

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
/**
 * calcolaPunteggio() è la subroutine che si occupa di calcolare il punteggio ottenuto da ciascun giocatore in un
 * turno, considerando i punti cfu di ciascuna carta e i bonus/malus di ogni personaggio e li salva in un array
 * allocato dinamicamente nella struttura turno
 * @param turno è la struttura di tipo Turno che rappresenta un turno di gioco
 * @param playerList è la lista dei giocatori nella partita
 * @param nPlayers è il numero di giocatori ing iococ
 */
void calcolaPunteggio(Turno *turno, Player *playerList, int nPlayers) {
	Player *headPlayer = playerList;
	CartaCfu *headCarte = turno->carteGiocate;
	int modifier = 0;

	if (turno->points == NULL) {
		turno->points = (int *)calloc(nPlayers, sizeof(int));
		if (turno->points == NULL){
			exit(ERR_FAIL_ALLOCATION_POINTS);
		}
	} else {
		turno->points = (int *)realloc(turno->points, nPlayers * sizeof(int));
		if (turno->points == NULL){
			exit(ERR_FAIL_ALLOCATION_POINTS);
		}
	}
	for (int i = 0; i < nPlayers; ++i) {
		modifier = headPlayer->character.bonusMalus[turno->cartaOstacolo->type];
		turno->points[i] = headCarte->cfu + modifier;
		headPlayer = headPlayer->nextPlayer;
		headCarte = headCarte->next;
	}
}

void printPuntiParziali(Turno *turno, Player *playerList, int nPlayers) {
	Player *pPlayer = playerList;

	printf("\nPunti Cfu del turno %d\n", turno->numTurno);
	for (int i = 0; i < nPlayers; ++i) {
		printf("%31s: %d\n", pPlayer->username, turno->points[i]);
		pPlayer = pPlayer->nextPlayer;
	}
	printf("+------------------------------+\n");
}

void winnersLosers(Turno *turno, Player *playersList, int nPlayers){
	Player *pPlayer = playersList;
	minMax(turno->points, nPlayers, &turno->cfuToLose, &turno->cfuToWin);

	for (int i = 0; i < nPlayers; ++i) {
		if (turno->points[i] == turno->cfuToWin){
			printf("Vincitore turno: %s", pPlayer->username);
		}
		if (turno->points[i] == turno->cfuToLose) {
			printf("Perdente turno: %s", pPlayer->username);
		}
		pPlayer = pPlayer->nextPlayer;
	}
}
// ============ CHIUSURA ===============================================================================================
/**
 * end() è la subroutine per chiudere una partita che si occupa di liberare la memoria
 * @param mazzoCfu
 * @param mazzoOstacoli
 * @param playerList
 * @param mazzoScarti
 */
void end(CartaCfu *mazzoCfu, CartaOstacolo *mazzoOstacoli, Player *playerList, CartaCfu *mazzoScarti){
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	exit(EXIT_SUCCESS);
}




