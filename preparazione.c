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

/**
 * openFile() è la funzione che apre i file forniti in modalità lettura
 *
 * @param filePath è il percorso al file da aprire
 * @return è un puntatore al file aperto
 */
FILE *openFile(char filePath[], char mode[]){
	FILE *fp = NULL;
	fp = fopen(filePath, mode);
	if (fp == NULL) exit(ERR_OPEN_FILE);
	return fp;
}


// ============ PERSONAGGI ================================================
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

/**
 * creaMazzoCfu() è la subroutine che legge il file delle carte CFU e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [carte.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaCfu *creaMazzoCfu(FILE *fp) {
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
 * creaMazzoOstacoli() è la subroutine che legge il file delle carte Ostacolo e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [ostacoli.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaOstacolo *creaMazzoOstacoli(FILE *fp) {
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
	return mazzo;
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

		if (read && (nGiocatori < 2 || nGiocatori > 4)){
			printf("Il numero di giocatori deve essere compreso tra 2 e 4:\n"
				   "\t controlla l'input o organizzatevi in squadre :)\n");
		}
	} while (!read && (nGiocatori < 2 || nGiocatori > 4));

	return nGiocatori;
}

/**
 * initGiocatori() è la funzione che, dati il numero dei giocatori e il mazzo di carte Cfu, restituisce una lista di
 * giocatori, occupandosi di inizializzarla, assegnando un username, personaggio e una mano di cinque carte iniziali
 * @param nGiocatori è il numero di giocatori che partecipano alla partita
 * @param mazzoCfu è un puntatore alla testa della lista che rappresenta il mazzo delle carte Cfu, passata alla
 * funzione per permettere l'assegnamento della mano iniziale di carte
 * @return la funzione restituisce listaGiocatori, una lista di strutture di tipo giocatore
 */
Giocatore *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu) {
	Giocatore *listaGiocatori = NULL;

	for (int i = 0; i < nGiocatori; ++i) {

	}
	return listaGiocatori;
}


