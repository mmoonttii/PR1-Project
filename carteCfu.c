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
	CartaCfu *next = NULL;

	while (mazzoCfu->next != NULL) {
		next = mazzoCfu->next;
		free(mazzoCfu);
		mazzoCfu = next;
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
 * distribuisciCarte è la subroutine che si occupa di distribuire carte ai giocatori fin quando questi non hanno
 * 5 carte in mano
 * @param mazzoCfu è un puntatore al puntatore alla prima carta del mazzo
 * @return un puntatore alla prima carta del mazzo
 */
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu){
	CartaCfu *head = mano;
	int counter = 0;

	// Se la mano non è vuota, scorro fino all'ultima carta, tenendo conto di quante carte sto scorrendo
	while (head != NULL && head->next != NULL){
		head = head->next;
		counter++;
	}
	// Fin quando la mano è composta da 5 carte o meno
	while (counter < 5){
		// Se il mazzo delle carte è vuoto
		if (mazzoCfu == NULL){
			// Rimescolo il mazzo degli scarti
			// TODO: mescola mazzo
			// mazzoCfu = mescolaMazzo(mazzoScarti);
		}
		// Se la testa è NULL, questo si verifica nel caso di una mano completamente vuota
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
			// La testa diventa la carta successiva
			head = head->next;
		}
		// Incremento il contatore che tiene traccia delle carte nella mano
		counter++;
	}
	head->next = NULL;

	return mano;
}

// ============ OUTPUT ========================================================

/**
 * printCarteCfu è la subroutine che si occupa di stampare una lista di carte e restituisce quante ne ha stampate
 * @param listaCarteCfu è la lista da stampare
 */
int printCarteCfu(CartaCfu *listaCarteCfu) {
	CartaCfu *head = listaCarteCfu; // Testa della lista
	int count = 0; // Contatore delle carte
	bool stop = true; // Condizione di uscita dalla stampa
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

	printf("\n==== CARTE CFU ====\n");
	// Finchè la condizione di stop è rispettata
	while (stop){
		// Stampa la struttura carta
		printf("\n[%d] Nome: %s,\n"
		       "|\tCFU: %d,\t Effetto: %s\n",
		       count, head->name, head->cfu, effetti[head->effect]);
		// In caso la prossima carta sia NULL, esci dal loop
		if (head->next == NULL){
			stop = false;
			// Altrimenti continua con la prossima carta
		} else {
			head = head->next;
		}
		count++;
	}
	// count è incrementato per indicare il numero di carte stampate
	count++;
	return count;
}

int choiceCarta(int count){
	int last = count - 1, // last è count - di uno per indicare l'indice dell'ultima carta anziche la dimensione
		choice;
	bool wrong = false;

	do {
		printf("\nQuale carta vuoi scegliere? [0-%d]\n>>> ", last);
		scanf("%d", &choice);

		if (choice < 0 || choice > last) {
			printf("\nIl numero inserito non è accettabile. Riprovare");
			wrong = true;
		} else {
			wrong = false;
		}
	} while (wrong);

	return choice;
}