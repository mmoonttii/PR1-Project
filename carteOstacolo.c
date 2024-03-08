//
// Created by Monti on 07/02/24.
//

#include "carteOstacolo.h"
#include "memoria.h"


// ============ MEMORY MANAGEMENT =============================================

// ============ LIST MANAGEMENT ===============================================/**

/**
 * creaMazzoOstacoli() è la subroutine che legge il file delle carte Ostacolo e crea una lista delle carte
 *
 * @param fOstacoli FILE *: è file da leggere [ostacoli.txt]
 * @return CartaOstacolo *: puntatore alla testa del mazzo
 */
CartaOstacolo *creaMazzoOstacoli(FILE *fOstacoli) {
	int nOstacoliTipo,  // numero di carte per ogni tipo
	    carte = 0;      // numero di carte nel mazzo

	Ostacolo tipo = 1;  // tipologia dell'ostacolo

	CartaOstacolo cartaOstacolo,    // Struttura ausiliaria dove salvare la carta letta
				  *mazzo = NULL,    // Puntatore al mazzo creato
				  *newCard = NULL;  // puntatore per l'allocazione di una nuova carta

	for (int i = 0; i < N_OSTACOLI; ++i) {  // Per ogni tipologia di ostacolo
		fscanf(fOstacoli, "%d\n", &nOstacoliTipo);  // Leggo quante carte ci sono di quel tipo

		for (int j = 0; j < nOstacoliTipo; ++j) {   // Per ogni carta di ogni tipo

			fscanf(fOstacoli, "%31[^\n]s", cartaOstacolo.name); // LEggo nome
			fscanf(fOstacoli, "\n");
			fscanf(fOstacoli, "%127[^\n]s", cartaOstacolo.desc); // Leggo descrizione
			fscanf(fOstacoli, "\n");
			cartaOstacolo.type = tipo;  // Salvo il tipo

			newCard = allocaCartaOstacolo(); // Alloco la nuova carta
			*newCard = cartaOstacolo;       // Riempio lo spazio allocato con la carta letta

			mazzo = ostacoloInTesta(mazzo, newCard); // Aggiungo la carta al mazzo
			carte++; // Aggiotno il numero di carte
		}
		tipo++; // Finite di leggere le carte di un tipo, passo al tipo successivo
	}
	return mazzo;
}

/**
 * ostacoloInTesta() è la subroutine che, data una lista di carte, ne aggiunge una nuova alla testa
 * @param mazzoOstacoli CartaOstacolo *: punta alla testa del mazzo a cui devo aggiungere la carta
 * @param cartaOstacolo CartaOstacolo *: punta alla carta che si vuole aggiungere in testa
 * @return CartaOstacolo *: punta alla nuova testa della lista
 */
CartaOstacolo *ostacoloInTesta(CartaOstacolo *mazzoOstacoli, CartaOstacolo *cartaOstacolo) {
	cartaOstacolo->next = mazzoOstacoli;
	mazzoOstacoli = cartaOstacolo;
	return mazzoOstacoli;
}

/**
 * mescolaMazzoOstacoli() è la subroutine che presa una lista di CarteOstacolo, la restituisce in un ordine randomizzato
 * @param mazzoDaMischiare CartaOstacolo **: doppio puntatore a testa del mazzo da mescolare
 * @return CartaOstacolo *: puntatore a testa del mazzo mischiato
 */
CartaOstacolo *mescolaMazzoOstacoli(CartaOstacolo **mazzoDaMischiare) {
	CartaOstacolo *mazzoMescolato = NULL,    // mazzoMescolato punta alla testa del mazzo già mescolato
	              *cartaEstratta  = NULL;    // cartaEstrtatta punta alla carta estratta randomicamente dal mazzo

	int count = 0,  // Conta quante carte sono nel mazzo da mischiare
	    randCarta;  // Posizione randomica della carta da estrarre

	count = contaOstacoli(*mazzoDaMischiare);

	while (*mazzoDaMischiare != NULL) {
		randCarta = randRange(0, count - 1); // Genero una posizione di una carta random
		cartaEstratta = indexEstraiCartaOstacolo(mazzoDaMischiare, randCarta);  // Estraggo tale carta random
		mazzoMescolato = ostacoloInTesta(mazzoMescolato, cartaEstratta); // Aggiungo la carta al mazzo
		count--;
	}
	return mazzoMescolato;
}

/**
 * indexEstraiCartaOstacolo() è la subroutine che poppa la index-esima carta Ostacolo dal mazzo
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore al mazzo da cui estrarre
 * @param index int: indice della carta da estrarre
 * @return CartaCfu *: puntatore alla carta poppata
 */
CartaOstacolo *indexEstraiCartaOstacolo(CartaOstacolo **mazzoOstacoli, int index) {
	CartaOstacolo *curr = *mazzoOstacoli, // curr punta all'elemento corrente della lista
				  *prev = *mazzoOstacoli, // prev punta all'elemento precedente della lista
	              *extracted = NULL; // extracted punta alla carta estratta dalla lista

	if (index == 0) {   // Se l'elemento della lista è in testa
		extracted = pescaCartaOstacolo(mazzoOstacoli); // Corrisponde a pescare la prima carta dal mazzo
	} else {
		// Scorro la lista di index volte, salvando ogni volta l'elemento precedente
		for (int i = 0; i < index; ++i) {
			prev = curr;
			curr = curr->next;
		}
		extracted = curr;   // l'elemento corrente va salvato in estratto
		prev->next = curr->next; // Salto l'elemento curr nella lista
		extracted->next = NULL;  // isolo il nodo estratto
	}

	return extracted;
}

/**
 * pescaCartaOstcolo() è la subroutine che data una lista di carte ostacolo ne estrae la prima e la
 * restituisce, aggiornando la lista con la nuova testa
 * @param mazzoOstacoli CartaOstacolo **: è un doppio puntatore alla testa della lista da cui pescare
 * @return CartaOstacolo *: puntatore alla carta pescata
 */
CartaOstacolo *pescaCartaOstacolo(CartaOstacolo **mazzoOstacoli) {
	CartaOstacolo *carta = NULL;
	carta = *mazzoOstacoli; // La carta da pescare è la prima del mazzo
	*mazzoOstacoli = (*mazzoOstacoli)->next;    // La nuova prima del mazzo, sarà la successiva
	carta->next = NULL; // Isolo la carta pescata
	return carta;
}

/**
 * ostacoloInCoda() è la subroutine che assegna a un giocatore una carta ostacolo.
 * La carta ostacolo è un doppio puntatore, per poter modificare a NULL il puntatore che puntava alla carta ostacolo
 * ora assegnata
 * @param pOstacolo è un doppio puntatore a lla carta ostacolo da assegnare
 * @param listaOstacoli è un doppio puntatore alla lista al quale aggiungere la carta ostacolo
 */
// Continuo a non capire cosa cazzo non vada
void ostacoloInCoda(CartaOstacolo *pOstacolo, CartaOstacolo **listaOstacoli){
	CartaOstacolo *headOstacoli = *listaOstacoli;
	if (*listaOstacoli == NULL) {
		*listaOstacoli = pOstacolo;
	} else {
		while (headOstacoli->next != NULL) {
			headOstacoli = headOstacoli->next;
		}
		headOstacoli->next = pOstacolo;
	}
	pOstacolo->next = NULL;
}
// ============ OUTPUT ========================================================
/**
 * printOstacoli è la subroutine che si occupa di stampare una lista di ostacoli
 * @param listaOstacoli è la lista da stampare
 */
void printOstacoli(CartaOstacolo *listaOstacoli){
	CartaOstacolo *head = listaOstacoli;
	char *types[] = {"null", "Studio", "Sopravvivenza", "Sociale", "Esame"};
	int count = 0;

	while (head != NULL){
		printf("\nNome: %s\n"
		       "|\tDescrizione: %s\n"
		       "|\tTipo: %s\n",
		       head->name, head->desc, types[head->type]);

		head = head->next;
		count++;
	}
}

int contaOstacoli(CartaOstacolo *listaOstacoli) {
	CartaOstacolo *headOstacoli = listaOstacoli;
	int count = 0;

	while (headOstacoli != NULL) {
		count++;
		headOstacoli = headOstacoli->next;
	}
	return count;
}

