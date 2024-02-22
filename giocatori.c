//
// Created by Monti on 07/02/24.
//

#include "giocatori.h"

// ============ PERSONAGGI =====================================================
/**
 * parseCharacters() è la funzione che legge il file dei personaggi e conserva le informazioni di ogni personaggio in
 * un'array di sturtture personaggio
 *
 * Una struttura characters nel campo bonusMalus abbiamo un array dove vengono salvati gli eventuali bonus e malus di
 * ogni giocatore per ogni tipologia di ostacolo
 *
 * @param fp è il file da leggere [personaggi.txt]
 * @param characters è l'indirizzo dell'array di strutture personaggio
*/
void parseCharacters(FILE *fp, Character characters[]) {
	// Per ogni personaggio
	for (int i = 0; i < N_PERSONAGGI; i++){
		// Per ogni ostacolo
		for (int j = 0; j < N_OSTACOLI; j++){
			fscanf(fp, "%d ", &characters[i].bonusMalus[j]);
		}
		fscanf(fp, "%[^\n]31s", characters[i].name);
		fscanf(fp, "\n");
	}
}

// ============ MEMORY MANAGEMENT =============================================
/**
 * allocaGiocatore è la funzione che si occupa di allocare in memoria lo spazio necessario per un giocatore
 * @return un puntatore alla locazione del giocatore appena allocato
 */
Player *allocaGiocatore() {
	Player *player = NULL;
	player = (Player *)malloc(sizeof(Player));
	if (player == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_PLAYER);
	}
	return player;
}

Player *freeGiocatore(Player *listagiocatori){
	Player *pAux = NULL;

	while (listagiocatori != NULL) {
		pAux = listagiocatori;
		listagiocatori = listagiocatori->nextPlayer;
		if (pAux != NULL){
			free(listagiocatori);
		}
	}
	listagiocatori = NULL;
	return listagiocatori;
}

// ============ IO ============================================================

/**
 * acquisisciNumGiocatore() è la funzione che acquisisce il numero dei partecipanti alla partita
 * La funzione si occupa di controllare che l'input sia valido e invita a ritentare in caso di input non valido
 * @return il numero di giocatori alla partita
 */
int acquisisciNumGiocatori() {
	int nGiocatori,
	    read = 0;

	do {
		printf("Quanti giocatori parteciperanno oggi? [2-4] ");
		read = scanf(" %d", &nGiocatori);

		if (nGiocatori < 2 || nGiocatori > 4){
			printf("Il numero di giocatori deve essere compreso tra 2 e 4:\n"
			       "\t controlla l'input o organizzatevi in squadre :)\n");
		}
	} while (nGiocatori < 2 || nGiocatori > 4);

	return nGiocatori;
}

void printGiocatore(Player *giocatore, bool stampaCarte) {
	printf("\nGiocatore:\n");
	printf("|\t Username: %s\n"
	       "|\t Cfu: %d\n"
	       "|\t Character: %s, Bonus-malus [",
	       giocatore->username, giocatore->cfu, giocatore->character.name);

	// Stampa ogni bonus/malus
	for (int j = 0; j < N_OSTACOLI; ++j) {
		printf("%d, ", giocatore->character.bonusMalus[j]);
	}
	printf("]\n");
	// Se stampaCarte stampa anche la sua lista di carte
	if (stampaCarte){
		printCarteCfu(giocatore->manoCarteCfu);
	}
	printf("+----------------------------------------\n");
}


/**
 * printGiocatori() è la subroutine che stampa la lista dei giocatori, il loro nome, personaggio, punteggio e mano
 * @param listaGiocatori è la lista di giocatori da stampare
 */
void printGiocatori(Player *listaGiocatori, bool stampaCarte) {
	Player *head = listaGiocatori;
	printf("\n=== GIOCATORI ===\n");

	// Fino a quando head indica un giocatore valido
	for (int i = 0; head != NULL; ++i) {
		// Stampa le informazioni
		printGiocatore(head, stampaCarte);
		// Scorri al giocatore successivo
		head = head->nextPlayer;
	}
}

// ============ LIST MANAGEMENT ===============================================

/**
 * initGiocatori() è la funzione che, dati il numero dei giocatori e il mazzo di carte Cfu, restituisce una lista di
 * giocatori, occupandosi di inizializzarla, assegnando un username, personaggio e una mano di cinque carte iniziali
 * @param nGiocatori è il numero di giocatori che partecipano alla partita
 * @param mazzoCfu è un puntatore alla testa della lista che rappresenta il mazzo delle carte Cfu, passata alla
 * funzione per permettere l'assegnamento della mano iniziale di carte
 * @param personaggi è l'array delle strutture dei personaggi
 * @return la funzione restituisce listaGiocatori, una lista di strutture di tipo giocatore
 */
Player *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu, Character personaggi[], CartaCfu *mazzoScarti)  {
	Player    *listaGiocatori = NULL,
	          *head           = NULL,
	          *newPlayer      = NULL,
	          aux;
	int       k; // Indice per l'accesso all'array di personaggi
	Character emptyCharacter  = {}; // Struttura personaggio ausiliaria vuota

	printf("\n=== PARTECIPANTI ===\n");
	// Per ogni giocatore
	for (int i = 0; i < nGiocatori; ++i) {
		// Richiedi username
		printf("\nGIOCATORE %d\n", i);
		printf("Inserire username: ");
		scanf(" %[^\n]31s", aux.username);

		// Inizializza la struttura
		aux.cfu           = 0; // Punteggio di partenza
		aux.manoCarteCfu  = NULL;
		aux.manoCarteCfu  = distribuisciCarte(aux.manoCarteCfu, mazzoCfu, &mazzoScarti); // Mano iniziale
		aux.listaCarteOstacolo = NULL;
		aux.nextPlayer         = NULL;

		do {
			// Genero una posizione casuale dall'array dei personaggi
			k = randRange(0, N_PERSONAGGI - 1);
			// Se personaggi[k] è uguale a emptyCharacter vuol dire che quel personaggio è già stato usato, quindi ne
			// genero un'altro, fino a quando non saranno diverso
		} while (strcmp(personaggi[k].name, emptyCharacter.name) == 0);

		aux.character = personaggi[k]; // Assegno il personaggio dall'array
		personaggi[k] = emptyCharacter; // Sostiuisco il posto nell'array con il personaggio vuoto


		// Creazione lista dei giocatori
		if (listaGiocatori == NULL) {
			listaGiocatori = allocaGiocatore();
			*listaGiocatori = aux;
			head = listaGiocatori;
		} else {
			head->nextPlayer = allocaGiocatore();
			*head->nextPlayer = aux;
			head = head->nextPlayer;
		}

	}
	return listaGiocatori;
}

/**
 * addPlayerInCoda() è la subroutine che, presi in input una lista e un nuovo nodo, COPIA tale nodo alla coda di
 * tale lista, assicurandosi che il puntatore next di questo sia NULL
 * @param playerList è la lista alla quale aggiungere il nodo
 * @param playerToAdd è il nodo da aggiungere
 * @return la testa della lista con il nodo aggiunto
 */
Player *addPlayerInCoda(Player *playerList, Player *playerToAdd){

	Player *head = playerList,
	       aux = *playerToAdd;
	aux.nextPlayer = NULL;

	// Se la lista è vuota
	if (playerList == NULL) {
		// Posso direttamente allocare il nuovo nodo sulla testa
		playerList = allocaGiocatore();
		*playerList = aux;
	} else {
		// Altrimenti scorro fino all'ultimo nodo e lo aggiungo dopo questo
		while (head->nextPlayer != NULL) {
			head = head->nextPlayer;
		}
		head->nextPlayer = allocaGiocatore();
		*(head->nextPlayer) = aux;
	}
	return playerList;
}
