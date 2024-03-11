//
// Created by Monti on 24/02/24.
//
#include "memoria.h"
#include "carteOstacolo.h"
#include "giocatori.h"
#include "turno.h"
// ============ CARTE CFU =============================================
 /**
 * allocaCartaCfu() è la subroutine che si occupa di allocare nell'heap lo spazio necessario a contenere una carta
 * @return CartaCfu *: puntatore alla locazione per una nuova carta
 */
CartaCfu *allocaCartaCfu() {
	CartaCfu *newCard = NULL;
	newCard = (CartaCfu *)calloc(1, sizeof(CartaCfu));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOC_CFU);
	}
	return newCard;
}

/**
 * freeCfu() è la funzione per liberare la memoria precedentemente allocata per una lista
 * @param mazzoCfu CartaCfu *: puntatore alla testa della lista
 */
CartaCfu *freeCfu(CartaCfu *mazzoCfu) {
	CartaCfu *pAux = NULL;

	// Salvo la posizione della carta attuale, salvo la carta successiva e la libero la attuale
	while (mazzoCfu != NULL) {
		pAux = mazzoCfu;
		mazzoCfu = mazzoCfu->next;
		if (pAux != NULL) {
			free(pAux);
		}
	}
	mazzoCfu = NULL;
	return mazzoCfu;
}

// ============ CARTE OSTACOLO =============================================
/**
 * allocaCartaOstacolo() è la funzione che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 * @return un puntatore alla locazione della nuova carta
 */
CartaOstacolo *allocaCartaOstacolo() {
	CartaOstacolo *newCard = NULL;
	newCard = (CartaOstacolo *)calloc(1, sizeof(CartaOstacolo));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOC_OSTACOLO);
	}
	return newCard;
} /**
 * freeOstacolo() è la funzione per liberare la memoria precedentemente allocata per una lista
 * @param mazzoOstacoli CartaOstacolo *: è il puntatore alla testa della lista
 */
CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli) {
	CartaOstacolo *pAux = NULL;

	while (mazzoOstacoli != NULL) {
		pAux = mazzoOstacoli;
		mazzoOstacoli = mazzoOstacoli->next;
		if (pAux != NULL) {
			free(pAux);
		}
	}
	mazzoOstacoli = NULL;
	return mazzoOstacoli;
}

// ========== GIOCATORI ========================================================
/**
 * allocaGiocatore() è la funzione che si occupa di allocare sull'heap lo spazio per un giocatore
 * @return Player *: puntatore allo spazio appena allocato
 */
Player *allocaGiocatore() {
	Player *player = NULL;
	// Allocazione
	player = (Player *)malloc(sizeof(Player));
	if (player == NULL) { // Se l'allocazione è fallita: libero la memoria già allocata ed esco dal programma con errore
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOC_PLAYER);
	}
	return player;
}

/**
 * freeGiocatore() è la funzione per liberare lo spazio allocato per ogni Player
 * @param playerList Player *: puntatore alla lista da liberare
 * @return Player *: NULL
 */
Player *freeGiocatore(Player *playerList) {
	Player *pAux = NULL; // Puntatore ausiliario di scorrimento lista

	while (playerList != NULL) {
		// Salvo l'elemento successivo, per liberare la memoria dell'attuale nodo
		pAux       = playerList;
		playerList = playerList->nextPlayer;
		if (pAux != NULL){
			free(pAux);
		}
	}
	// Mi assicuro che il puntatore sia NULL
	playerList   = NULL;
	return playerList;
}

// ========== ARRAYS ===================================================================================================
/**
 * freeIntArr() è la subroutine per liberare un array di interi
 * @param arr array da liberare
 * @return
 */
int *freeIntArr(int *arr) {
	free(arr);
	arr = NULL;

	return arr;
}

/**
 * allocaArrBool() è la funzione che alloca un array di n booleani inizializzato a false
 * @param n numero di booleani nell'array
 * @return puntatore all'array
 */
bool *allocaArrBool(int n) {
	bool *arr = NULL;

	arr = (bool *)calloc(sizeof(bool), n);
	if (arr == NULL) {
		exit(ERR_FAIL_ALLOC_BOOLS);
	}

	for (int i = 0; i < n; ++i) {
		arr[i] = false;
	}
	return arr;
}

/**
 * freeBoolARR() libera la memoria allocata per un array di booleani
 * @param arr
 * @return
 */
bool *freeBoolArr(bool *arr) {
	free(arr);
	arr = NULL;
	return arr;
}

// ========== FREE ALL =================================================================================================
/**
 * freeAll() è la subroutine che si occupa di liberare la memoria allocata durante il gioco
 * @param mazzoCfu CartaCfu *: mazzo carte Cfu
 * @param mazzoScarti CartaCfu *: mazzo scarti
 * @param mazzoOstacoli CartaOstacolo *: mazzoOstacoli
 * @param playerList Player *: lidta di giocatori
 */
void freeAll(CartaCfu *mazzoCfu, CartaCfu *mazzoScarti, CartaOstacolo *mazzoOstacoli, Player *playerList) {
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
}