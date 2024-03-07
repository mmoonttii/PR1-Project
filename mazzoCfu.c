//
// Created by Monti on 24/02/24.
//

#include "mazzoCfu.h"
#include "memoria.h"

/**
 * creaMazzoCfu() è la subroutine per la lettura del file di carte e la creazione di una lista di Carte
 *
 * @param fp FILE *: file da leggere [carte.txt]
 * @return CartaCfu *: puntatore alla testa del mazzo
 */
CartaCfu *creaMazzoCfu(FILE *fp) {
	int occurences = 0, // Numero di carte dello stesso tipo
	    carte      = 0, // Numero di carte nel mazzo
	    read       = 0; // Numero di letture avvenute con successo
	CartaCfu cartaCfu,  // Struttura ausiliaria dove viene salvata la nuova carta letta
			 *newCard = NULL,    // Puntatore per l'allocazione di una nuova carta
			 *mazzo   = NULL;    // Puntatore all'inizio del mazzo [return]

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
				*newCard = cartaCfu;        // Inizializza spazio allocato con dati letti

				cartaCfuInCoda(&mazzo, newCard);

				occurences--;   // Il numero di carte dello stesso tipo ora lette è diminuito
				carte++;        // Il numero delle carte totali nel mazzo è aumentato
			}
		} else {
			// In caso di lettura fallita
			printf("\n--printf scurrile rimosso--\n");
		}
	} while (read == 4); // Se la lettura fallisce vuol dire che è terminato il file

	printf("\nCarte lette %d\n", carte);

	return mazzo;
}

/**
 * mescolaMazzo() è la subroutine che presa una lista di CarteCfu, la restituisce in un ordine randomizzato
 * @param mazzoDaMischiare CartaCfu **: doppio puntatore a testa di mazzo da mescolare
 * @return CartaCfu *: puntatore a testa del mazzo mischiato
 */
CartaCfu *mescolaMazzo(CartaCfu **mazzoDaMischiare) {
	CartaCfu *mazzoMescolato = NULL,
	         *headMescolato  = NULL,
	         *headMischiare  = *mazzoDaMischiare,
	         *cartaEstratta  = NULL,
	         *prev           = NULL;
	int count = 0,
	    randCarta;

	count = contaCarteCfu(*mazzoDaMischiare);

	while (*mazzoDaMischiare != NULL) {
		randCarta = randRange(0, count - 1); // Genero una posizione di una carta random
		cartaEstratta = indexEstraiCartaCfu(mazzoDaMischiare, randCarta); // Estraggo tale carta random
		mazzoMescolato = cartaCfuInTesta(mazzoMescolato, cartaEstratta); // Aggiungo la carta al mazzo
		count--;
	}
	return mazzoMescolato;
}

CartaCfu *cartaCfuInTesta(CartaCfu *mazzoCfu, CartaCfu *cartaCfu) {
	cartaCfu->next = mazzoCfu;
	mazzoCfu = cartaCfu;
	return mazzoCfu;
}

int contaCarteCfu(CartaCfu *mazzoCfu) {
	int count = 0;
	while (mazzoCfu != NULL){
		mazzoCfu = mazzoCfu->next;
		count++;
	}
	return count;
}

CartaCfu *indexEstraiCartaCfu(CartaCfu **mazzoCfu, int index) {
	CartaCfu *head = *mazzoCfu,
	         *prev = *mazzoCfu,
	         *extracted = NULL;

	if (index == 0) {
		extracted = *mazzoCfu;
		*mazzoCfu = extracted->next;
		extracted->next = NULL;
	} else {
		for (int i = 0; i < index; ++i) {
			prev = head;
			head = head->next;
		}
		extracted = head;
		prev->next = head->next;
		extracted->next = NULL;
	}

	return extracted;
}

/**
 * Data il puntatore a una carta Cfu estrae tale carta se è nella lista
 * @param mazzoCfu
 * @param cartaCfu
 * @return
 */
CartaCfu *estraiCartaCfu(CartaCfu **mazzoCfu, CartaCfu *cartaCfu) {
	CartaCfu *head = *mazzoCfu,
			 *prev = *mazzoCfu,
			 *extracted = NULL;

	bool leave = false;

	while (!leave) {
		if (*mazzoCfu == cartaCfu) {
			extracted = *mazzoCfu;
			*mazzoCfu  = (*mazzoCfu)->next;
			extracted->next = NULL;
			leave = true;
		} else if (prev->next == cartaCfu) {
			extracted       = prev->next;
			prev->next      = extracted->next;
			extracted->next = NULL;
			leave = true;
		} else {
			prev = head;
			head = head->next;
			leave = false;
			if (head == NULL) {
				printf("\nLa carta non è stata trovata nella lista");
				leave = true;
			}
		}
	}
	return extracted;
}

/**
 * cartaCfuInCoda() è la subroutine per aggiungere un nodo di tipo CartaCfu alla fine di una lista dello stesso tipo
 * @param mazzoCfu CartaCfu **: è un doppio puntatore alla testa della lista a cui si vuole aggiungere la carta
 * @param cartaCfu CartaCfu *: è un puntatore alla carta da aggiungere
 */
void cartaCfuInCoda(CartaCfu **mazzoCfu, CartaCfu *cartaCfu) {
	CartaCfu *curr = *mazzoCfu; // curr è il puntatore all'elemento corrente della lista

	if (*mazzoCfu == NULL) {    // Se la lista è vuota
		*mazzoCfu = cartaCfu;   // La carta occupa la posizione di testa
	} else {                    // Altrimenti, se sono già presenti nodi
		while (curr->next != NULL){ // Itero fino a quando non trovo un elemento il quale next è vuoto
			curr = curr->next;
		}
		curr->next = cartaCfu;  // A questo punto posso aggiungere la carta in coda
	}
}

/**
 * distribuisciCarte è la subroutine che si occupa di distribuire carte ai giocatori fin quando questi non hanno
 * 5 carte in mano
 * @param mazzoCfu è un puntatore al puntatore alla prima carta del mazzo
 * @return un puntatore alla prima carta del mazzo
 */
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti) {
	CartaCfu *cartaCfu = NULL;
	int counter = 0;
	counter = contaCarteCfu(mano);

	// Fin quando la mano è composta da 5 carte o meno
	while (counter < CARTE_PER_MANO){
		// Se il mazzo delle carte è vuoto, lo rimescolo
		if (*mazzoCfu == NULL){
			*mazzoCfu = mescolaMazzo(mazzoScarti);
		}

		cartaCfu = indexEstraiCartaCfu(mazzoCfu, 0); // Estraggo dal mazzo la prima carta
		cartaCfuInCoda(&mano, cartaCfu);    // E la inserisco in coda alla mano
		counter++; // Incremento il contatore delle carte in mano
	}
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
	*daScartare = NULL;
}

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
void printMano(CartaCfu *listaCarteCfu) {
	CartaCfu *head = listaCarteCfu; // Testa della lista
	bool tutteIstantanee = true;
	int i = 0;

	printf("\n==== CARTE CFU ====\n");
	// Finchè la condizione di stop è rispettata
	while (head != NULL){
		printf("[%d] ", i);
		printSingleCartaCfu(head);

		if (isIstantanea(head)) {
			printf("| CARTA ISTANTANEA - non puoi giocarla in questa fase del turno\n");
		}
		head = head->next;
		i++;
	}

}

bool isIstantanea(CartaCfu *cartaCfu) {
	bool ris;
	ris = (cartaCfu->cfu == 0 && cartaCfu->effect != SCARTAP) ? true : false;
	return ris;
}

bool tutteIstantaneeCheck(CartaCfu *cartaCfu) {
	CartaCfu *head = cartaCfu;
	bool tutteIstantanee = true;

	while (head != NULL) {
		if (!isIstantanea(head)) {
			tutteIstantanee = false;
		}
		head = head->next;
	}
	return tutteIstantanee;
}
CartaCfu *findCartaCfu(CartaCfu **mazzoCfu, int index) {
	CartaCfu *head = *mazzoCfu,
	         *prev  = *mazzoCfu;

	// Se la carta scelta non è la prima, quindi choice != 0
	for (int i = 0; i < index; i++) {
		// Scorro fino alla prossima carta, salvando la posizione della carta precedente
		prev = head;
		head = head->next;
	}
	return head;
}