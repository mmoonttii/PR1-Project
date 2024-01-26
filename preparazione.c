//
// Created by Monti on 26/01/24.
//

#include "preparazione.h"

// ============ PERSONAGGI ================================================

/** parseCharacters() è la funzione che legge il file dei personaggi e conserva le informazioni in un'array di
 * sturtture personaggio
 *
 * @param fp è il file da leggere [personaggi.txt]
 * @param characters è l'indirizzo dell'array di strutture personaggio
*/
void parseCharacters(FILE *fp, Personaggio characters[]) {
	for (int i = 0; i < N_PERSONAGGI; i++){
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
 * @return è un puntatore alla locazione della nuova carta
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
 * @return è un puntatore alla testa del mazzo
 */
CartaCfu *mazzoCfu(FILE *fp) {
	int read = 0;
	int occurences = 0;
	CartaCfu *newCard = NULL;
	CartaCfu *head = NULL;

	do {
		newCard = nuovaCfu();
		read = fscanf(fp, "%d %d %d %[^\n]31s", &occurences, &newCard->effect, &newCard->cfu, newCard->name);

		if (head == NULL){
			// TODO:
		} else {
			// TODO:
		}
	} while (read == 4);
	return NULL;
}


