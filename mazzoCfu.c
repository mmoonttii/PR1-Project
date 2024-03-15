#include "mazzoCfu.h"
#include "memoria.h"

// ========== LIST MANAGEMENT ==========================================================================================
/**
 * creaMazzoCfu() è la subroutine per la lettura del file di carte e la creazione di una lista di Carte
 *
 * @param fCfu FILE *: file da leggere [carte.txt]
 * @return CartaCfu *: puntatore alla testa del mazzo
 */
CartaCfu *creaMazzoCfu(FILE *fCfu) {
	int occurences = 0, // Numero di carte dello stesso tipo
	    carte      = 0, // Numero di carte nel mazzo
	    read       = 0; // Numero di letture avvenute con successo
	CartaCfu cartaCfu,  // Struttura ausiliaria dove viene salvata la nuova carta letta
			 *newCard = NULL,    // Puntatore per l'allocazione di una nuova carta
			 *mazzo   = NULL;    // Puntatore all'inizio del mazzo [return]

	do {
		// Lettura riga dal file, se read == 4 lettura success
		read = 0;
		read = fscanf(fCfu, "%d %d %d %[^\n]31s", &occurences, &cartaCfu.effect, &cartaCfu.cfu, cartaCfu.name);
		fscanf(fCfu, "\n");

		cartaCfu.next = NULL;

		// Se lettura success
		if (read == 4) {
			// Per ogni carte di tipo x
			while (occurences > 0) {
				newCard = allocaCartaCfu(); // Alloca nuova carta e salva in var
				*newCard = cartaCfu;        // Inizializza spazio allocato con dati letti

				mazzo = cartaCfuInTesta(mazzo, newCard); // Aggiungo la carta al mazzo

				occurences--;   // Il numero di carte dello stesso tipo ora lette è diminuito
				carte++;        // Il numero delle carte totali nel mazzo è aumentato
			}
		} else {
			// In caso di lettura fallita
			printf("\n--printf scurrile rimosso--\n");
		}
	} while (read == 4); // Se la lettura fallisce vuol dire che è terminato il file e posso uscire dal ciclo

	printf("\nCarte lette %d\n", carte);

	return mazzo;
}

/**
 * mescolaMazzo() è la subroutine che presa una lista di CarteCfu, la restituisce in un ordine randomizzato
 * @param mazzoDaMischiare CartaCfu **: doppio puntatore a testa di mazzo da mescolare
 * @return CartaCfu *: puntatore a testa del mazzo mischiato
 */
CartaCfu *mescolaMazzo(CartaCfu **mazzoDaMischiare) {
	CartaCfu *mazzoMescolato = NULL,    // mazzoMescolato punta alla testa del mazzo già mescolato
	         *cartaEstratta  = NULL;    // cartaEstrtatta punta alla carta estratta randomicamente dal mazzo

	int count = 0,  // Conta quante carte sono nel mazzo da mischiare
	    randCarta;  // Posizione randomica della carta da estrarre

	count = contaCarteCfu(*mazzoDaMischiare);

	while (*mazzoDaMischiare != NULL) {
		randCarta = randRange(0, count - 1); // Genero una posizione di una carta random
		cartaEstratta = indexEstraiCartaCfu(mazzoDaMischiare, randCarta);  // Estraggo tale carta random
		mazzoMescolato = cartaCfuInTesta(mazzoMescolato, cartaEstratta); // Aggiungo la carta al mazzo
		count--;
	}
	return mazzoMescolato;
}

/**
 * cartaCfuInTesta() è la subroutine che, data una lista di carte, ne aggiunge una nuova nella testa
 * @param mazzoCfu CartaCfu *: punta alla testa del mazzo a cui devo aggiungere la carta
 * @param cartaCfu CartaCfu *: punta alla carta che si vuole aggiungere in testa
 * @return CartaCfu *: punta alla nuova testa della lista
 */
CartaCfu *cartaCfuInTesta(CartaCfu *mazzoCfu, CartaCfu *cartaCfu) {
	cartaCfu->next = mazzoCfu; // Alla carta segue il mazzo già esistente
	mazzoCfu = cartaCfu;       // Salvo la nuova testa nella variabile mazzoCfu
	return mazzoCfu;
}

/**
 * findCartaCfu() trova la i-esima carta nel mazzo e la restituisce, senza rimuoverla dal mazzo
 * @param mazzoCfu CartaCfu *: punta al mazzo dove cercare la i-esima carat
 * @param index int: indice della carta da restituire
 * @return CartaCfu *: puntatore alla carta Cfu nell'i-esima posizione
 */
CartaCfu *findCartaCfu(CartaCfu *mazzoCfu, int index) {
	CartaCfu *curr = mazzoCfu;

	// Se la carta scelta non è la prima, quindi choice != 0
	for (int i = 0; i < index; i++) {
		// Scorro fino alla prossima carta
		curr = curr->next;
	}
	return curr;
}

/**
 * indexEstraiCartaCfu() è la subroutine che poppa la index-esima carta Cfu dal mazzo
 * @param mazzoCfu CartaCfu **: doppiopuntatore al mazzo da cui estrarre la carta
 * @param index int: indice della carta da estrarre
 * @return CartaCfu *: puntatore alla carta che è stata poppata
 */
CartaCfu *indexEstraiCartaCfu(CartaCfu **mazzoCfu, int index) {
	CartaCfu *curr = *mazzoCfu, // curr punta all'elemento corrente della lista
	         *prev = *mazzoCfu, // prev punta all'elemento precedente della lista
	         *extracted = NULL; // extracted punta alla carta estratta dalla lista

	if (index == 0) {   // Se l'elemento della lista è in testa
		extracted = *mazzoCfu;  // salvo l'elemento di testa
		*mazzoCfu = extracted->next; // il mazzo prende la carta successiva
		extracted->next = NULL; // isolo la carta che ho estratto

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
 * Data il puntatore a una carta Cfu estrae tale carta se è nella lista
 * @param mazzoCfu CartaCfu **: doppio puntatore alla lista da cui estrarre la carta
 * @param cartaCfu CartaCfu *: puntatore adalla crata da estrarre
 * @return CartaCfu *: carta estratta
 */
CartaCfu *estraiCartaCfu(CartaCfu **mazzoCfu, CartaCfu *cartaCfu) {
	CartaCfu *curr      = NULL,
			 *prev      = NULL,
			 *extracted = NULL;

	bool leave = false;

	curr = *mazzoCfu;
	prev = *mazzoCfu;

	while (!leave) {
		// Se la testa del mazzo è la carta richiesta, posso estrarla
		if (*mazzoCfu == cartaCfu) {
			extracted = *mazzoCfu;
			*mazzoCfu = (*mazzoCfu)->next;
			extracted->next = NULL;
			leave = true;
		// Se prev.next è la carta richiesta posso estrarla
		} else if (prev->next == cartaCfu) {
			extracted       = prev->next;
			prev->next      = prev->next->next;
			extracted->next = NULL;
			leave = true;
		// Altrimenti passo alla prossima carta
		} else {
			prev  = curr;
			curr  = curr->next;
			leave = false;
			if (curr == NULL) {
				printf("\nLa carta non è stata trovata nella lista");
				extracted = NULL;
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
		while (curr->next != NULL){ // Itero fino a quando non trovo un elemento il quale next è nullo
			curr = curr->next;
		}
		curr->next = cartaCfu;  // A questo punto posso aggiungere la carta in coda
	}
}

/**
 * estraiTesta() estrae la carta di indice 0 da un mazzo di carte cfu
 * @param mazzoCfu CartaCfu **: mazzo da cui estrarre la testa
 * @return CartaCfu *: carta estratta
 */
CartaCfu *estraiTesta(CartaCfu **mazzoCfu) {
	CartaCfu *headCard = NULL;
	// Salvo la testa della lista
	headCard = *mazzoCfu;
	// Assegno come testa della lista il next
	*mazzoCfu = (*mazzoCfu)->next;
	return headCard;
}

/**
 * distribuisciCarte() è la subroutine che si occupa di distribuire carte ai giocatori fin quando questi non hanno
 * 5 carte in mano
 * @param mano CartaCfu *: puntatore alla mano del giocatore, al quale si vogliono aggiungere carte
 * @param mazzoCfu CartaCfu **: doppio puntatore al mazzo di pesca
 * @param mazzoScarti CartaCfu **: doppio puntatore al mazzo degli scarti, passato per eventuale rimescolamento
 * @return CartaCfu *: puntatore alla prima carta della mano
 */
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti) {
	CartaCfu *cartaCfu = NULL; // Carta da assegnare
	int counter = 0;

	counter = contaCarteCfu(mano); // Conto le carte inizialmente in mano

	// Fin quando la mano è composta da 5 carte o meno
	while (counter < CARTE_PER_MANO){
		// Se il mazzo delle carte è vuoto, lo rimescolo
		if (*mazzoCfu == NULL){
			*mazzoCfu = mescolaMazzo(mazzoScarti);
		}

		cartaCfu = estraiTesta(mazzoCfu); // Estraggo dal mazzo la prima carta
		mano = cartaCfuInTesta(mano, cartaCfu); // E la inserisco in testa alla mano
		counter++; // Incremento il contatore delle carte in mano
	}
	return mano;
}

/**
 * scartaCarte() è la subroutine che sposta una lista di carte in un'altra
 * @param daScartare CartaCfu **: doppio puntatore alla lsita di carte da scartare
 * @param mazzoScarti CartaCfu *: puntatore alla lista a cui aggiungere le carte
 * @return CartaCfu *: lista di carte con aggiunta delle carte scartate
 */
CartaCfu *scartaCarte(CartaCfu **daScartare, CartaCfu *mazzoScarti) {
	CartaCfu *curr = *daScartare;

	// Ciclo fino all'ultima carta da scartare
	while (curr->next != NULL) {
		curr = curr->next;
	}

	// Accodo alla carta il mazzo degli scarti
	curr->next = mazzoScarti;

	// Mazzoscarti prende la prima carta della nuova lista
	mazzoScarti = *daScartare;

	// Mi assicuro che la vecchia lista punti a NULL
	*daScartare = NULL;

	return mazzoScarti;
}

// ========== OUTPUT ===================================================================================================
/**
 * printSigleCartaCfu() stampa una singola carta Cfu
 * @param pCfu CartaCfu *: carta Cfu da stampare
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
 * printCarteCfu() si occupa di stampare una lista di carte
 * @param listaCarteCfu CartaCfu *: lista di carte da stampare
 */
void printMano(CartaCfu *listaCarteCfu) {
	CartaCfu *curr = listaCarteCfu;
	int i          = 0;

	printf("\n==== CARTE CFU ====\n");
	// Finchè la condizione di stop è rispettata
	while (curr != NULL){
		printf("[%d] ", i);
		printSingleCartaCfu(curr);

		if (isIstantanea(curr)) {
			printf("| CARTA ISTANTANEA - non puoi giocarla in questa fase del turno\n");
		}
		curr = curr->next;
		i++;
	}
}

// ========== VARIE ====================================================================================================
/**
 * contaCarteCfu() è la subroutine che conta da quanti nodi è composta una lista di CarteCfu
 * @param mazzoCfu CartaCfu *: punta alla testa della lista da contare
 * @return int: numero di nodi nella lista
 */
int contaCarteCfu(CartaCfu *mazzoCfu) {
	int count = 0;
	// Scorro la lista fino a quando non è terminata
	while (mazzoCfu != NULL){
		mazzoCfu = mazzoCfu->next;
		count++; // Aggiorno il contatore dei nodi nella lista
	}
	return count;
}

/**
 * isIstantanea() controlla se la carta in questione è istantanea
 * @param cartaCfu CartaCfu *: è la carta da controllare
 * @return bool: true se la carta è istantanea, false altrimenti
 */
bool isIstantanea(CartaCfu *cartaCfu) {
	bool ris = false;
	// Se l'effetto della carta è compreso tra AUMENTA e DIROTTA allora la carta è istantanea
	if (cartaCfu->effect >=AUMENTA) {
		ris = true;
	}
	return ris;
}

/**
 * tutteIstantaneeCheck() controlla se tutte le carte della lista passata sono istantanee oppure no
 * @param cartaCfu CartaCfu *: è la lista da controllare
 * @return bool: true se tutte le carte sono istantanee, false se almeno una non è istantanea
 */
bool tutteIstantaneeCheck(CartaCfu *cartaCfu) {
	CartaCfu *curr = cartaCfu;  // Puntatore a elemento corrente della lista
	bool tutteIstantanee = true; // Valore di ritorno

	// Ciclando la lista
	while (curr != NULL) {
		// Se una carta non è istantanea
		if (!isIstantanea(curr)) {
			// Allora posso settare il valore di ritorno a false
			tutteIstantanee = false;
		}
		// Passo alla prossima carta
		curr = curr->next;
	}
	return tutteIstantanee;
}