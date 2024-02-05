//
// Created by Monti on 05/02/24.
//

#include "memManagement.h"

// ============ CARTE CFU =====================================================
/**
 * allocaCartaCfu() è la subroutine che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 *
 * @return un puntatore alla locazione della nuova carta
 */
CartaCfu *allocaCartaCfu() {
	CartaCfu *newCard = NULL;
	newCard = (CartaCfu *)malloc(sizeof(CartaCfu));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_CFU);
	}
	return newCard;
}

/**
 * freeCfu() è la funzione per liberare la memoria precedentemente allocata per una lista
 * @param mazzoCfu è il puntatore alla testa della lista
 */
CartaCfu *freeCfu(CartaCfu *mazzoCfu) {
	CartaCfu *next = NULL;

	while (mazzoCfu->next != NULL) {
		next = mazzoCfu->next;
		free(mazzoCfu);
		mazzoCfu = next;
	}

	mazzoCfu = NULL;
	return mazzoCfu;

}

// ============ CARTE OSTACOLO ================================================
/**
 * allocaCartaOstacolo() è la funzione che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 *
 * @return un puntatore alla locazione della nuova carta
 */
CartaOstacolo *allocaCartaOstacolo() {
	CartaOstacolo *newCard = NULL;
	newCard = (CartaOstacolo *)malloc(sizeof(CartaOstacolo));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_OSTACOLO);
	}
	return newCard;
}

/**
 * freeOstacolo() è la funzione per liberare la memoria precedentemente allocata per una lista
 * @param mazzoOstacoli è il puntatore alla testa della lista
 */
CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli) {
	CartaOstacolo *next = NULL;

	while (mazzoOstacoli->next != NULL) {
		next = mazzoOstacoli->next;
		free(mazzoOstacoli);
		mazzoOstacoli = next;
	}

	mazzoOstacoli = NULL;

	return mazzoOstacoli;
}

// ============ GIOCATORI =====================================================
/**
 * allocaGiocatore è la funzione che si occupa di allocare in memoria lo spazio necessario per un giocatore
 * @return un puntatore alla locazione del giocatore appena allocato
 */
Giocatore *allocaGiocatore() {
	Giocatore *player = NULL;
	player = (Giocatore *)malloc(sizeof(Giocatore));
	if (player == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_PLAYER);
	}
	return player;
}

Giocatore *freeGiocatore(Giocatore *listagiocatori){
	Giocatore *next = NULL;

	while (listagiocatori->nextPlayer != NULL) {
		next = listagiocatori->nextPlayer;
		free(listagiocatori);
		listagiocatori = next;
	}
	listagiocatori = NULL;
	return listagiocatori;
}