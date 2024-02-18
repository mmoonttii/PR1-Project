#include "turno.h"
#include "carteOstacolo.h"

// ============ TURNO - FASE 1 ================================================

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
void giocaCarta(CartaCfu *manoCarteCfu, CartaCfu *listaCarteGiocate) {
	CartaCfu *headMano    = manoCarteCfu,
	         *headGiocate = listaCarteGiocate,
	         *choosenCard = NULL,
	         *prev        = NULL,
			 *auxP        = NULL;
	int      choice       = 0,
	         count;


	count  = printCarteCfu(manoCarteCfu);
	choice = choiceCarta(count);

	// Se la carta scelta non è la prima
	if (choice != 0) {
		for (int i = 0; i < choice; i++) {
			// Scorro fino alla prossima carta, salvando la posizione della carta precedente
			prev     = headMano;
			headMano = headMano->next;
		}
		choosenCard = headMano;
		prev->next = choosenCard->next;
		choosenCard->next = NULL;
	// Se dobbiamo estrarre la prima carta,
	} else {
		manoCarteCfu = headMano->next; // la testa della mano prende la carta successiva
		choosenCard = headMano; // La carta scelta è la testa della mano
	}

	// Se la lista delle carte giocate è vuota, allora la lista è la prima carta
	if (listaCarteGiocate == NULL) {
		listaCarteGiocate = choosenCard;
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

	printGiocatore(choosenPlayer, false);
}

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