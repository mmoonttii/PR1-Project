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
 * @param mazzoCfu è il puntatore alla testa della lista
 */
CartaCfu *freeCfu(CartaCfu *mazzoCfu) {
	CartaCfu *pAux = NULL;

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
 *
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
 * @param mazzoOstacoli è il puntatore alla testa della lista
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
 * allocaGiocatore è la funzione che si occupa di allocare in memoria lo spazio necessario per un giocatore
 * @return un puntatore alla locazione del giocatore appena allocato
 */
Player *allocaGiocatore() {
	Player *player = NULL;
	player = (Player *)malloc(sizeof(Player));
	if (player == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOC_PLAYER);
	}
	return player;
}
Player *freeGiocatore(Player *listagiocatori){
	Player *pAux = NULL;

	while (listagiocatori != NULL) {
		pAux = listagiocatori;
		listagiocatori = listagiocatori->nextPlayer;
		if (pAux != NULL){
			free(pAux);
		}
	}
	listagiocatori = NULL;
	return listagiocatori;
}

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

bool *freeBoolArr(bool *arr) {
	free(arr);
	arr = NULL;
	return arr;
}

void freeAll(CartaCfu *mazzoCfu, CartaCfu *mazzoScarti, CartaOstacolo *mazzoOstacoli, Player *playerList) {
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
}