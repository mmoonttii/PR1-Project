//
// Created by Monti on 07/02/24.
//

#include "carteCfu.h"

// ============ MEMORY MANAGEMENT =============================================
/**
 * allocaCartaCfu() è la subroutine che si occupa di allocare in memoria lo spazio necessario a contenere una carta
 *
 * @return un puntatore alla locazione della nuova carta
 */
CartaCfu *allocaCartaCfu() {
	CartaCfu *newCard = NULL;
	newCard = (CartaCfu *)calloc(1, sizeof(CartaCfu));
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
	CartaCfu *pAux = NULL;

	while (mazzoCfu != NULL) {
		pAux = mazzoCfu;
		mazzoCfu = mazzoCfu->next;
		if (pAux != NULL) {
			free(mazzoCfu);
		}
	}
	mazzoCfu = NULL;
	return mazzoCfu;

}

// ============ LIST MANAGEMENT ===============================================/**
/**
 * creaMazzoCfu() è la subroutine che legge il file delle carte CFU e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [carte.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaCfu *creaMazzoCfu(FILE *fp) {
	int read       = 0,       // Numero di letture avvenute con successo
	    occurences = 0, // Numero di carte dello stesso tipo
	    carte      = 0,      // Numero di carte nel mazzo
	    pos        = 0;         // Posizione in cui inserire una nuova carta
	CartaCfu cartaCfu,        // Struttura ausiliaria dove viene salvata la nuova carta letta
	*newCard = NULL, // Puntatore per l'allocazione di una nuova carta
	*mazzo   = NULL,   // Puntatore all'inizio del mazzo [return]
	*auxP    = NULL,    // Puntatore ausiliario
	*head    = NULL;   // Puntatore alla testa

	do {
		// Lettura riga dal file, se read == 4 lettura success
		read = 0;
		read = fscanf(fp, "%d %d %d %[^\n]31s", &occurences, &cartaCfu.effect, &cartaCfu.cfu, cartaCfu.name);
		fscanf(fp, "\n");

		cartaCfu.next = NULL;

		// Se lettura success
		if (read == 4) {
			// Per ogni carte di tipo x
			while (occurences > 0) {
				newCard = allocaCartaCfu(); // Alloca nuova carta e salva in var
				*newCard = cartaCfu; // Inizializza spazio allocato con dati letti

				if (mazzo == NULL) {
					// Se il mazzo è vuoto la carta letta diventa la testa del mazzo
					mazzo = newCard;
				} else {
					// Altrimenti genera posizione in cui aggiungere la carta
					// -1 aggiungere in testa
					// n aggiungere dopo carta di indice n
					// carte - 1 aggiungere in coda
					pos = randRange(-1, carte - 1);

					// Aggiunta in testa
					if (pos == -1) {
						auxP  = mazzo; // Salvo testa del mazzo precedente
						mazzo = newCard; // La nuova testa del mazzo diventa la carta appena allocata
						mazzo->next = auxP; // Dopo la nuova testa collego il resto del mazzo vecchio

						// Aggiunta in coda
					} else if (pos == carte - 1){
						// Raggiungo l'ultima posizione della lista
						while (head != NULL){
							// Salvo il puntatore alla struttura attuale
							auxP = head;
							head = head->next;
							// Quando head è NULL, sono andato troppo avanti
						}
						// Quinid ritorno alla posizione precedente
						head = auxP;
						// Il campo next dell'attuale head, ultimo della lista, può accogliere la carta appena allocata
						head->next = newCard;
						newCard->next = NULL;
					} else {
						// Scorro fino alla carta in posizione pos
						for (int i = 0; i < pos; ++i) {
							head = head->next;
						}
						auxP = head->next; // Salvo il puntatore alla carta successiva a pos
						head->next    = newCard; // La successiva alla carta pos, sarà la nuova carta
						newCard->next = auxP; // Dopo la nuova carta riaggiungo il resto del mazzo
					}
				}
				head = mazzo;
				occurences--; // Il numero di carte dello stesso tipo ora lette è diminuito
				carte++; // Il numero delle carte totali nel mazzo è aumentato
			}
		} else {
			// In caso di lettura fallita
			printf("\n--printf scurrile rimosso--\n");
		}
	} while (read == 4);
	printf("\nCarte lette %d\n", carte);

	return mazzo;
}

/**
 * mescolaMazzo() è la subroutine che dato una lista di Carte CFU la restituisce in un ordine diverso
 * @param mazzoScarti è il mazzo da mescolare
 * @return
 */
CartaCfu *mescolaMazzo(CartaCfu **mazzoScarti) {
	CartaCfu *mazzoMescolato = NULL,
	         *headMescolato  = NULL,
	         *headScarti     = *mazzoScarti,
	         *cartaEstratta  = NULL,
	         *prev           = NULL;
	int count    = 0,
	    randCarta;

	// Conto le carte nella lista
	while (headScarti->next != NULL){
		headScarti = headScarti->next;
		count++;
	}
	headScarti = *mazzoScarti;

	while (headScarti != NULL) {
		randCarta = randRange(0, count);// Genero una posizione di una carta random
		// Scorro fino a tale carta random
		for (int i = 0; i < randCarta; ++i) {
			prev       = headScarti;
			headScarti = headScarti->next;
		}

		cartaEstratta = headScarti;

		// Se la carta era in prima posizione
		if (randCarta == 0) {
			// la testa del mazzo diventa la carta successiva
			headScarti = headScarti->next;
		} else {
			// Altrimenti la sua posizione viene saltata
			prev->next = headScarti->next;
		}

		// Se il mazzo mescolato è vuoto
		if (mazzoMescolato == NULL) {
			// Posso mettere la carta appena estratta in prima posizione
			mazzoMescolato = cartaEstratta;
		} else {
			headMescolato = mazzoMescolato;
			// Altrimenti ciclo fino alla prima posizione libera e la inserisco
			while (headMescolato->next != NULL) {
				headMescolato = headMescolato->next;
			}
			headMescolato->next = cartaEstratta;
		}
	}
	return mazzoMescolato;
}
/**
 * distribuisciCarte è la subroutine che si occupa di distribuire carte ai giocatori fin quando questi non hanno
 * 5 carte in mano
 * @param mazzoCfu è un puntatore al puntatore alla prima carta del mazzo
 * @return un puntatore alla prima carta del mazzo
 */
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti) {
	CartaCfu *head = mano;
	int counter = 0;

	// Se la mano non è vuota, scorro fino all'ultima carta, tenendo conto di quante carte sto scorrendo
	if (head != NULL) {
		while (head->next != NULL) {
			head = head->next;
			counter++;
		}
		counter++;
	}
	// Fin quando la mano è composta da 5 carte o meno
	while (counter < CARTE_PER_MANO){
		// Se il mazzo delle carte è vuoto
		if (*mazzoCfu == NULL){
			*mazzoCfu = mescolaMazzo(mazzoScarti);
		}
		// Se la testa è NULL (nel caso di una mano completamente vuota)
		if (head == NULL){
			// La testa della mano prende la testa del mazzo
			head = *mazzoCfu;
			// La testa del mazzo diventa la carta successiva a quella appena "distribuita"
			*mazzoCfu = (*mazzoCfu)->next;
			// Salvo la nuova testa come inizio della mano
			mano = head;

			// Se la testa non è null, quindi abbiamo già almeno una carta in mano
		} else {
			// La prossima carta della mano diventa la prima carta del mazzo
			head->next = *mazzoCfu;
			// La prima carta del mazzo diventa la carta successiva a quella appena "distribuita"
			*mazzoCfu = (*mazzoCfu)->next;
			// La testa diventa la carta appena ricevuta
			head = head->next;
		}
		counter++; // Incremento il contatore che tiene traccia delle carte nella mano
	}
	head->next = NULL;

	return mano;
}

/**
 * scartaCarte() è la subroutine che scarta una lista di carte, senza liberare la memoria
 * che rappresenta
 * @param daScartare
 * @param mazzoScarti
 */
void scartaCarte(CartaCfu **daScartare, CartaCfu **mazzoScarti) {
	CartaCfu *head = *mazzoScarti;

	// Se il mazzo degli scarti è vuoto
	if (head == NULL){
		// Il mazzzo degli scarti prende la lista di carte da scartare
		*mazzoScarti = *daScartare;
	} else {
		// Altrimenti, scorro fino alla fine del mazzo
		while (head->next != NULL) {
			head = head->next;
		}
		// E aggiungo in coda la lista di carte
		head->next = *daScartare;
	}
	// Mi assicuro che la lista di carte scartate sia NULL
	daScartare = NULL;
}


// ============ OUTPUT ========================================================

/**
 * printSigleCartaCfu() è la subroutine che stampa una singola carta Cfu
 * @param pCfu è la carta Cfu da stampare
 */
void printSingleCartaCfu(CartaCfu *pCfu) {
	char *effetti[] = {"Carta senza effetto",
	                   "Scarta una carta CFU punto e aggiungi il suo punteggio quello del turno",
	                   "Guarda la mano di un collega e ruba una carta a scelta",
	                   "Scambia questa carta con quella di un altro giocatore, purché senza effetto",
	                   "Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno",
	                   "Scarta da uno a tre carte dalla tua mano",
	                   "Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base",
	                   "Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti)",
	                   "Guarda due carte in cima al mazzo, prendine una e scarta l’altra",
	                   "Scambia le carte punto di due giocatori qualsiasi",
	                   "Annulla gli effetti di tutte le carte punto durante il turno",
	                   "Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta",
	                   "Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta",
	                   "Inverti punteggio minimo e massimo del turno",
	                   "Metti la carta Ostacolo che stai per prendere in fondo al mazzo",
	                   "Dai la carta che stai per prendere ad un altro giocatore a tua scelta"
	};
	// Stampa la struttura carta
	printf("%s\n"
	       "|\tCFU: %d\t Effetto: %s\n",
	       pCfu->name, pCfu->cfu, effetti[pCfu->effect]);
}
/**
 * printCarteCfu() è la subroutine che si occupa di stampare una lista di carte e restituisce quante ne ha stampate
 * @param listaCarteCfu è la lista di carte da stampare
 */
void printCarteCfu(CartaCfu *listaCarteCfu) {
	CartaCfu *head = listaCarteCfu; // Testa della lista
	bool stop = true; // Condizione di uscita dalla stampa

	printf("\n==== CARTE CFU ====\n");
	// Finchè la condizione di stop è rispettata
	while (stop){
		printSingleCartaCfu(head);
		// In caso la prossima carta sia NULL, esci dal loop
		if (head->next == NULL){
			stop = false;
			// Altrimenti continua con la prossima carta
		} else {
			head = head->next;
		}
	}
}
