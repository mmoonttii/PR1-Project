//
// Created by Monti on 26/01/24.
//

#include "preparazione.h"

// ============ UTILITIES =================================================

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


// ============ PERSONAGGI ================================================

/** openCharacters() è la funzione che apre il file dei personaggi
 *
 * @param filePath è il percorso al file "personaggi.txt" da aprire
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
		for (int j = 0; j < N_OSTACOLI; j++){
			fscanf(fp, "%d ", &characters[i].bonusMalus[j]);
		}
		fscanf(fp, "%[^\n]31s", characters[i].name);
		fscanf(fp, "\n");
	}
}


// ============ CARTE CFU =================================================
/**
 * openCfu() è la funzione che apre il file delle carte CFU
 * @param filePath è il percorso al file "carte.txt" da aprire
 * @return puntatore al file aperto
 */
FILE *openCfu(char *filePath) {
	FILE *fp = NULL;
	fp = fopen(filePath, "r");
	if (fp == NULL) exit(ERR_OPEN_FILE_CFU);
	return fp;
}


/**
 * nuovaCfu() è la subroutine che si occupa di allocare in memoria lo spazio necessario a contenere una carta
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
	int read = 0,       // Numero di letture avvenute con successo
		occurences = 0, // Numero di carte dello stesso tipo
		carte = 0,      // Numero di carte nel mazzo
		random;         // Posizione in cui inserire una nuova carta
	CartaCfu cartaCfu,        // Struttura ausiliaria dove viene salvata la nuova carta letta
			 *newCard = NULL, // Puntatore per l'allocazione di una nuova carta
			 *mazzo = NULL,   // Puntatore all'inizio del mazzo [return]
			 *auxP = NULL,    // Puntatore ausiliario
			 *head = NULL;   // Puntatore alla testa

	do {
		read = 0;
		read = fscanf(fp, "%d %d %d %[^\n]31s", &occurences, &cartaCfu.effect, &cartaCfu.cfu, cartaCfu.name);
		fscanf(fp, "\n");

		while (occurences > 0){
			// Caso in cui il mazzo sia vuoto
			if (mazzo == NULL){
				mazzo = nuovaCfu(); // Viene allocato lo spazio per una nuova carta
				*mazzo = cartaCfu; // La nuova carta viene inizializzata
				mazzo->next = NULL; // Inizializzo anche il puntatore alla carta successiva
				occurences--; // Il numero di carte dello stesso tipo ora lette è diminuito
				carte++; // Il numero delle carte totali nel mazzo è aumentato
				head = mazzo; // Inizializzo il puntatore alla testa con la testa del mazzo
			} else {
				// random è la posizione nel quale inserire una nuova carta, dove
				// 0 è la posizione immediatamente successiva alla prima carta del mazzo
				// -1 è la posizione come nuova prima carta del mazzo
				random = randRange(-1, carte - 1);

				// Posizionamento carta all'inizio del mazzo
				if (random == -1) {
					newCard = nuovaCfu(); // Viene allocata la nuova carta
					*newCard = cartaCfu; // Lo spazio allocato viene inizializzato alla carta letta
					newCard->next = mazzo; // Il pointer next della nuova carta prende tutto il mazzo
					mazzo = newCard; // L'inizio del mazzo è aggiornato alla nuova carta
					occurences--; // Il numero di carte dello stesso tipo ora lette è diminuito
					carte++; // Il numero delle carte totali nel mazzo è aumentato

				} else {

					// Posizionamento carta nel mezzo del mazzo
					// Entro nel mazzo fino alla posizione generata randomicamente
					for (int i = 0; i < random; ++i) {
						head = head->next;
					}
					auxP = head->next; // Salvo il puntatore alla carta che risulterà la successiva a quella letta
					newCard = nuovaCfu(); // Alloco la memoria per nuova carta
					*newCard = cartaCfu; // Inizializzo la nuova carta
					head->next = newCard; // La nuova carta viene inserita in posizione successiva a head
					newCard->next = auxP; // Il resto della lista viene messo successivamente alla nuova carta
					occurences--; // Il numero di carte dello stesso tipo ora lette è diminuito
					carte++; // Il numero delle carte totali nel mazzo è aumentato
				}
			}
		}
	} while (read == 4);
	return mazzo;
}




// ============ CARTE OSTACOLO ================================================
/**
 * openOstacoli() è la funzione che apre il file delle carte Ostacolo
 * @param filePath è il percorso al file "ostacoli.txt" da aprire
 * @return puntatore al file aperto
 */
FILE *openOstacoli(char *filePath) {
	FILE *fp = NULL;
	fp = fopen(filePath, "r");
	if (fp == NULL) exit(ERR_OPEN_FILE_OSTACOLI);
	return fp;
}

/**
 * nuovoOstacolo() è la funzione che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 *
 * @return un puntatore alla locazione della nuova carta
 */
CartaOstacolo *nuovoOstacolo() {
	CartaOstacolo *newCard = NULL;
	newCard = (CartaOstacolo *)malloc(sizeof(CartaOstacolo));
	if (newCard == NULL) {
		// TODO: FREE MEM
		exit(ERR_FAIL_ALLOCATION_OSTACOLO);
	}
	return newCard;
}

/**
 * mazzoOstacoli() è la subroutine che legge il file delle carte Ostacolo e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [ostacoli.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaOstacolo *mazzoOstacoli(FILE *fp) {
	int nOstacoliTipo,
		carte = 0,
		random;
	Ostacolo tipo = 1;
	CartaOstacolo cartaOstacolo,
				  *mazzo = NULL,
				  *newCard = NULL,
				  *head = NULL,
				  *auxP = NULL;

	for (int i = 0; i < N_OSTACOLI; ++i) {
		fscanf(fp, "%d\n", &nOstacoliTipo);

		for (int j = 0; j < nOstacoliTipo; ++j) {

			fscanf(fp, "%31s", cartaOstacolo.name);
			fscanf(fp, "\n");
			fscanf(fp, "%127s", cartaOstacolo.desc);
			fscanf(fp, "\n");
			cartaOstacolo.type = tipo;

			if (mazzo == NULL){
				mazzo = nuovoOstacolo();
				*mazzo = cartaOstacolo;
				mazzo->next = NULL;
				head = mazzo;
				carte++;
			} else {
				// random è la posizione nel quale inserire una nuova carta, dove
				// 0 è la posizione immediatamente successiva alla prima carta del mazzo
				// -1 è la posizione come nuova prima carta del mazzo
				random = randRange(-1, carte - 1);

				// Posizionamento carta all'inizio del mazzo
				if (random == -1) {
					newCard = nuovoOstacolo(); // Viene allocata la nuova carta
					*newCard = cartaOstacolo; // Lo spazio allocato viene inizializzato alla carta letta
					newCard->next = mazzo; // Il pointer next della nuova carta prende tutto il mazzo
					mazzo = newCard; // L'inizio del mazzo è aggiornato alla nuova carta
					carte++; // Il numero delle carte totali nel mazzo è aumentato

				} else {

					// Posizionamento carta nel mezzo del mazzo
					// Entro nel mazzo fino alla posizione generata randomicamente
					for (int k = 0; k < random; ++k) {
						head = head->next;
					}
					auxP = head->next; // Salvo il puntatore alla carta che risulterà la successiva a quella letta
					newCard = nuovoOstacolo(); // Alloco la memoria per nuova carta
					*newCard = cartaOstacolo; // Inizializzo la nuova carta
					head->next = newCard; // La nuova carta viene inserita in posizione successiva a head
					newCard->next = auxP; // Il resto della lista viene messo successivamente alla nuova carta
					carte++; // Il numero delle carte totali nel mazzo è aumentato
				}
			}
		}
		tipo++;
	}
	return NULL;
}



