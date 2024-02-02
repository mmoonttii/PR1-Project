//
// Created by Monti on 26/01/24.
//

#include "preparazione.h"

// ============ PERSONAGGI ================================================

/** openCharacters() è la funzione che apre il file dei personaggi
 *
 * @param filePath è i percorso del file "personaggi.txt" da aprire
 * @return è un puntatore al file aperto
 */
FILE *openCharacters(char filePath[]){
	FILE *fp = NULL;
	fp = fopen(filePath, "r");
	if (fp == NULL) exit(ERR_OPEN_FILE_CHARACTERS);
	return fp;
}

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
void parseCharacters(FILE *fp, Personaggio characters[]) {
	// Per ogni personaggio
	for (int i = 0; i < N_PERSONAGGI; i++){
		// Per ogni ostacolo
		for (int j = 0; j <= N_OSTACOLI; j++){
			fscanf(fp, "%d ", &characters[i].bonusMalus[j]);
		}
		fscanf(fp, "%[^\n]31s", characters[i].name);
		fscanf(fp, "\n");
	}
}


// ============ CARTE CFU =================================================
/** nuovaCfu() è la subroutine che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 *
 * @return un puntatore alla locazione della nuova carta
 */
CartaCfu *nuovaCfu() {
	CartaCfu *newCard = NULL;
	newCard = (CartaCfu *)malloc(sizeof(CartaCfu));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_CFU);
	}
	return newCard;
}

/** mazzoCfu() è la subroutine che legge il file delle carte CFU e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [carte.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaCfu *mazzoCfu(FILE *fp) {
	int read = 0;
	int occurences = 0;
	int carte = 0;
	int      random;
	CartaCfu cartaCfu;
	CartaCfu *newCard = NULL;
	CartaCfu *mazzo = NULL;
	CartaCfu *head = mazzo;
	CartaCfu *auxP = NULL;

	do {
		read = 0;
		read = fscanf(fp, "%d %d %d %[^\n]31s", &occurences, &cartaCfu.effect, &cartaCfu.cfu, cartaCfu.name);

		while (occurences > 0){
			// Caso in cui il mazzo sia vuoto
			if (mazzo == NULL){
				mazzo = nuovaCfu();
				*mazzo = cartaCfu;
				mazzo->next = NULL;
				occurences--;
				carte++;
			} else {
				random = randRange(-1, carte - 1);

				// Posizionamento carta all'inizio del mazzo
				if (random == -1) {
					newCard = nuovaCfu();
					*newCard = cartaCfu;
					newCard->next = mazzo;
					mazzo = newCard;
					occurences--;
					carte++;

				} else {

					// Posizionamento carta nel mezzo del mazzo
					for (int i = 0; i < random; ++i) {
						head = head->next;
					}
					auxP = head->next;
					newCard = nuovaCfu();
					*newCard = cartaCfu;
					head->next = newCard;
					newCard->next = auxP;
					occurences--;
					carte++;
				}
			}
		}
	} while (read == 4);
	return NULL;
}

/**
 * randRange() è la funzione che, restiuisce un numero pseudorandom intero compreso tra due interi
 * @param min valore inferiore del range
 * @param max valore superiore del range
 * @return intero pseudorandom generato
 */
int randRange(int min, int max) {
	int random;
	random = min + rand() % (max - min + 1);
	return random;
}



