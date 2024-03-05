//
// Created by Monti on 07/02/24.
//

#include "carteOstacolo.h"
#include "memoria.h"


// ============ MEMORY MANAGEMENT =============================================

// ============ LIST MANAGEMENT ===============================================/**

/**
 * creaMazzoOstacoli() è la subroutine che legge il file delle carte Ostacolo e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [ostacoli.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaOstacolo *creaMazzoOstacoli(FILE *fp) {
	int      nOstacoliTipo,
	         carte = 0,
	         pos;
	Ostacolo tipo = 1;
	CartaOstacolo cartaOstacolo,
			*mazzo = NULL,
			*newCard = NULL,
			*head = NULL,
			*auxP = NULL;

	for (int i = 0; i < N_OSTACOLI; ++i) {
		fscanf(fp, "%d\n", &nOstacoliTipo);

		for (int j = 0; j < nOstacoliTipo; ++j) {

			fscanf(fp, "%31[^\n]s", cartaOstacolo.name);
			fscanf(fp, "\n");
			fscanf(fp, "%127[^\n]s", cartaOstacolo.desc);
			fscanf(fp, "\n");
			cartaOstacolo.type = tipo;

			newCard = allocaCartaOstacolo();
			*newCard = cartaOstacolo;

			if (mazzo == NULL){
				mazzo = newCard;
				mazzo->next = NULL;
			} else {
				// pos è la posizione nel quale inserire una nuova carta, dove
				// 0 è la posizione immediatamente successiva alla prima carta del mazzo
				// -1 è la posizione come nuova prima carta del mazzo
				pos = randRange(-1, carte - 1);

				// Posizionamento carta all'inizio del mazzo
				if (pos == -1) {
					auxP = mazzo;
					mazzo = newCard;
					mazzo->next = auxP;
				} else {
					for (int k = 0; head != NULL && k < pos; ++k) {
						head = head->next;
					}
					if (head != NULL) {
						auxP = head->next; // Salvo il puntatore alla carta che risulterà la successiva a quella letta
						head->next = newCard; // La nuova carta viene inserita in posizione successiva a head
						newCard->next = auxP; // Il resto della lista viene messo successivamente alla nuova carta
					} else {
						head = newCard;
						head->next = NULL;
					}
				}
			}
			head = mazzo;
			carte++;
		}
		tipo++;
	}
	return mazzo;
}

/**
 * pescaCartaOstcolo() è la subroutine che data in input una lista di carte ostacolo ne estrae la prima e la
 * restituisce, aggiornando la lista con la nuova testa
 * @param mazzoOstacoli è un puntatore al puntatore alla testa della lista da cui pescare
 * @return il primo elemento del mazzo
 */
CartaOstacolo *pescaCartaOstacolo(CartaOstacolo **mazzoOstacoli) {
	CartaOstacolo *carta = NULL;
	carta = *mazzoOstacoli;
	*mazzoOstacoli = (*mazzoOstacoli)->next;
	carta->next = NULL;
	return carta;
}

/**
 * ostacoloInCoda() è la subroutine che assegna a un giocatore una carta ostacolo.
 * La carta ostacolo è un doppio puntatore, per poter modificare a NULL il puntatore che puntava alla carta ostacolo
 * ora assegnata
 * @param pOstacolo è un doppio puntatore a lla carta ostacolo da assegnare
 * @param listaOstacoli è un doppio puntatore alla lista al quale aggiungere la carta ostacolo
 */
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

