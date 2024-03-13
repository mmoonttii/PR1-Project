#include "effetti.h"

/*
typedef enum {
	NESSUNO, 	Carta senza effetto
	SCARTAP, 	Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno
	RUBA, 		Guarda la mano di un collega e ruba una carta a scelta
	SCAMBIADS, 	Scambia questa carta con quella di un altro giocatore, purché senza effetto
	SCARTAE, 	Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno
	SCARTAC, 	Scarta da uno a tre carte dalla tua mano
	SCAMBIAP, 	Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base
	DOPPIOE, 	Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti)
	SBIRCIA, 	Guarda due carte in cima al mazzo, prendine una e scarta l’altra
	SCAMBIAC,   Scambia le carte punto di due giocatori qualsiasi
	ANNULLA, 	Annulla gli effetti di tutte le carte punto durante il turno
	AUMENTA, 	Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta
	DIMINUISCI, Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta
	INVERTI, 	Inverti punteggio minimo e massimo del turno
	SALVA, 		Metti la carta Ostacolo che stai per prendere in fondo al mazzo
	DIROTTA 	Dai la carta che stai per prendere ad un altro giocatore a tua scelta
} Effect
 */

/**
 * gestioneEffetti() controlla le carte giocate e chiama la risoluzione degli effetti nell'ordine corretto
 * @param nPlayers int: numero di giocatori in gioco
 * @param playerList Player *: lista dei giocatori
 * @param mazzoCfu CartaCfu **: doppio puntatore al mazzo di pesca Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore al mazzo degli scarti
 * @param turno Turno *: struttura turno
 * @param checkDOPPIOE bool *: puntatore al booleano di controllo attivazione effetto DOPPIOE
 */
void gestioneEffetti(int nPlayers, Player *playerList,
					 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
					 Turno *turno, bool *checkDOPPIOE) {
	bool *arrRisolte    = NULL;     // Array controllo risoluzione carta
	int cfuMax          = INT_MIN;  // Cfu maggiori tra le carte giocate
	Player *currPlayer  = NULL;     // Puntatore al giocatore corrente
	CartaCfu *currCarta = NULL;     // Puntatore alla carta corrente

	arrRisolte = allocaArrBool(nPlayers); // Alloco l'array di controllo risoluzione carte
	currPlayer = playerList;    // Init di currPlayer a playerList

	// Per ogni giocatore
	for (int j = 0; j < nPlayers; ++j) {
		currCarta = turno->carteGiocate;
		cfuMax    = INT_MIN;

		// Trovo quale carta che è stata giocata, del quale non è stato ancora risolto l'effetto, ha i cfu massimi
		for (int i = 0; i < nPlayers; i++) {
			if (!arrRisolte[i] && currCarta->cfu > cfuMax) {
				cfuMax = currCarta->cfu;
			}
			currCarta = currCarta->next;
		}

		currCarta  = turno->carteGiocate;
		currPlayer = playerList;

		// Trovati i cfu massimi, trovo quale carta, non ancora risolta ha questi cfu
		for (int i = 0; i < nPlayers; ++i) {
			if (!arrRisolte[i] && currCarta->cfu == cfuMax) {
				// E ne risolvo l'effetto
				if (currCarta->effect != ANNULLA) {
					arrRisolte[i] = risolviEffetti(i, currPlayer, nPlayers, playerList,
					                               currCarta, mazzoCfu, mazzoScarti,
					                               turno,
					                               arrRisolte, checkDOPPIOE);
					enterClear();
				} else {
					// L'effetto annulla annulla tutti gli effetti successivi, quindi posso far finire i cicli
					printf("La carta %s di %s ha annullato tutti gli altri effetti delle carte\n",
					       currCarta->name, currPlayer->username);
					i = nPlayers;
					j = nPlayers;
					enterClear();
				}
			}
			currCarta  = currCarta->next;
			currPlayer = currPlayer->nextPlayer;
		}
	}
}

/**
 * risolviEffetti() chiama le subroutine corrispondenti all'effetto da risolvere
 * @param iPlayer int: indice del giocatore del quale si sta risolvendo l'effetto
 * @param currPlayer Player *: puntatore al giocatore del quale si sta risolvendo l'effetto
 * @param nPlayers int: numero di giocatori in gioco
 * @param playerList Player *: lista di giocatori
 * @param currCarta CartaCfu *: carta in risoluzione
 * @param mazzoCfu CartaCfu **: doppio puntatore a mazzo di pesca carte cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore a mazzo scarti Carte Cfu
 * @param turno Turno *: puntatore a struttura turno
 * @param arrRisolte bool []: array risoluzione effetti
 * @param checkDOPPIOE bool *: puntatore all'indicatore di attivazione effetto DOPPIOE
 * @return bool: indicatore risoluzione effetto carta
 */
bool risolviEffetti(int iPlayer, Player *currPlayer,
					int nPlayers, Player *playerList,
                    CartaCfu *currCarta,
					CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                    Turno *turno,
                    bool arrRisolte[], bool *checkDOPPIOE) {
	printf("Risolvo effetto della carta %s di %s\n", currPlayer->username, currCarta->name);

	switch (currCarta->effect) {
		case SCARTAP:
			effettoSCARTAP(iPlayer, currPlayer, mazzoScarti, turno);
			break;
		case RUBA:
			effettoRUBA(playerList, currPlayer);
			break;
		case SCAMBIADS:
			effettoSCAMBIADS(iPlayer, currPlayer, nPlayers, playerList, currCarta, turno, arrRisolte);
			break;
		case SCARTAE:
			effettoSCARTAE(iPlayer, currPlayer, mazzoScarti, turno);
			break;
		case SCARTAC:
			effettoSCARTAC(currPlayer, mazzoScarti);
			break;
		case SCAMBIAP:
			effettoSCAMBIAP(nPlayers, playerList, turno);
			break;
		case DOPPIOE:
			effettoDOPPIOE(checkDOPPIOE);
			break;
		case SBIRCIA:
			effettoSBIRCIA(mazzoCfu, currPlayer, mazzoScarti);
			break;
		case SCAMBIAC:
			effettoSCAMBIAC(&(turno->carteGiocate), playerList, arrRisolte, nPlayers);
			break;
		default:
			printf("\nErrore\n");
			break;
	}
	return true;
}

// ==== EFFETTI ========================================================================================================
/**
 * effettoSCARTAP subroutine per l'effetto SCARTAP: \n
 * 	Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno \n
 * @param mazzoScarti CartaCfu **: mazzo degli scarti
 * @param pPlayer Player *: giocatore a cui appartiene la carta
 * @param iPlayer int: indice giocatore
 * @param turno Turno *: struttura turno
 */
void effettoSCARTAP(int iPlayer, Player *pPlayer,
					CartaCfu **mazzoScarti, Turno *turno) {
	bool emptyMano        = false,  // Bool presenza carte in mano
		 allIstantanee    = true;   // Bool solo carte istantanee in mano
	CartaCfu *choosenCard = NULL,   // Carta scelta
			 *currMano    = NULL;   // Carta corrente della lista

	currMano = pPlayer->manoCarteCfu;

	if (pPlayer->manoCarteCfu==NULL) { // Flaggo che la mano è vuota
		emptyMano = true;
	}

	// Controllo che si abbia almeno una carta non istantanea
	allIstantanee = tutteIstantaneeCheck(currMano);

	if (!allIstantanee && !emptyMano) { // Se si può scartare almeno una carta
		// Permetto di scegliere una carta: utilizzo la subroutine chooseCarta() disabilitando le funzioni che
		// permettono di ripescare la mano di carte
		choosenCard = chooseCarta(&currMano, NULL, NULL, SPAREGGIO);

		// Estraggo la carta dalla mano
		choosenCard = estraiCartaCfu(&currMano, choosenCard);

		// Assegno i punti della carta estratta ai punti del giocatore
		turno->points[iPlayer] += choosenCard->cfu;

		printf("%s ha adesso %d Cfu in più, per un totale di %d Cfu\n",
			   pPlayer->username, choosenCard->cfu, turno->points[iPlayer]);

		// Scarto la carta
		*mazzoScarti = cartaCfuInTesta(*mazzoScarti, choosenCard);

	} else { // Se non si hanno carte ammesse
		printf("Non puoi scartare nessuna carta, mi dispiace\n");
	}
}

// =====================================================================================================================
/**
 * effettoRUBA() subroutine per effetto RUBA:\n
 * 	Guarda la mano di un collega e ruba una carta a scelta
 * @param playerList Player *: lista dei giocatori
 * @param pPlayer Player *: player a cui appartiene la carta
 */
void effettoRUBA(Player *playerList, Player *pPlayer) {
	Player      *currPlayer = NULL; // Player corrente della lista
	CartaCfu    *stolenCard = NULL; // Carta rubata
	int i      = 0,
		choice = 0;
	bool reTry = false;
	currPlayer = playerList;

	printf("\nA chi vuoi rubare una carta?\n");
	// Stampo i giocatori evitando di stampare il proprio giocatore
	while (currPlayer != NULL){
		if (strcmp(currPlayer->username, pPlayer->username) != 0) {
			printf("[%d] %s\n", i, currPlayer->username);
		}
		i++;
	}

	// Acquiscp la scelta dell'utente, raggiungo il player selezionato, richiedo l'input se si è selezionato se stessi
	do {
		currPlayer = playerList;
		choice = acquisisciInputInt(0, i - 1);
		for (int j = 0; j < choice; ++j) {
				currPlayer = currPlayer->nextPlayer;
		}
		if (currPlayer == pPlayer) {
			printf("\nNon puoi rubare una carta a te stesso\n");
			reTry = true;
		}
	} while (reTry);

	// Rubo la carta con la funzione steal
	stolenCard = steal(currPlayer);
	// Aggiungo la carta alla mano del giocatore
	cartaCfuInTesta(pPlayer->manoCarteCfu, stolenCard);

}

/**
 * steal() ruba la carta al giocatore selezionato
 * @param pPlayer Player *: giocatore a cui rubare la carta
 * @return CartaCfu *: carta rubata
 */
CartaCfu *steal(Player *pPlayer) {
	CartaCfu *curr       = pPlayer->manoCarteCfu,
			 *stolenCard = NULL;

	int i      = 0,
	    choice = 0;

	// Mostra carte che si possono rubare
	printf("\nQuale carta vuoi rubare a %s?\n", pPlayer->username);
	while (curr != NULL) {
		printf("[%d] ", i);
		printSingleCartaCfu(curr);
		curr = curr->next;
		i++;
	}

	choice = acquisisciInputInt(0, i);
	stolenCard = indexEstraiCartaCfu(&(pPlayer->manoCarteCfu), choice);

	return stolenCard;
}

// =====================================================================================================================
/**
 * effettoSCAMBIADS() subroutine per effetto SCAMBIADS:\n
 * 	Scambia questa carta con quella di un altro giocatore, purché senza effetto
 * @param iPlayer int: indice del giocatore che sta attuando l'effetto
 * @param pPlayer Player *: puntatore al player che sta risolvendo l'effetto
 * @param nPlayers int: numero di player in gioco
 * @param playerList Player *: lista di playerzz
 * @param turno Turno *:
 * @param arrRisolte bool *:
 */
void effettoSCAMBIADS(int iPlayer, Player *pPlayer,
					  int nPlayers, Player *playerList,
					  CartaCfu *cartaPlayer,
					  Turno *turno, bool arrRisolte[]) {
	Player *playerScambio = NULL; // puntatore in cui salvare il giocatore con cui si effettua lo scambio

	CartaCfu *currCarte    = NULL, // variabile ausiliaria per scorrere la lista delle carte giocate
			 *pChoosenCard = NULL, // puntatore per salvare la carta scelta dal giocatore
			 choosenCardAux,
			 cartaPlayerAux;
	bool check;

	int i         = 0, // Counter cicli
		choice    = 0, // Indice carta scelta
		noScambio = 0; // Countere carte non scambiabili

	currCarte = turno->carteGiocate; // Assegno la lista delle carte giocate
	i = 0;

	printf("\nCarte giocate questo turno: ");
	while (currCarte != NULL) {
		if (i == iPlayer) { // Non si può scambiare la carta con se stessa
			noScambio++;    // Incremento le carte non scambiabili
		} else {
			printf("\n[%d] Carta di %s:\n", i, pPlayer->username);
			printSingleCartaCfu(currCarte);

			if (currCarte->effect != NESSUNO) { // Se la carta ha effetti non può essere scambiata
				printf("| Questa carta ha un effetto, non puoi prenderla\n");
				noScambio++; // Incremento carte non scambiabili
			}
		}
		i++;
		currCarte  = currCarte->next;
		playerList = playerList->nextPlayer;
	}

	if (i == noScambio) { // Se il numero di carte coincide con il numero di carte non scambiabili termina la funzione
		printf("Non puoi scambiare nessuna carta\n");
	} else {
		do {
			// Acquisisco con quale carta il giocatore vuole scambiare la propria
			do {
				choice = acquisisciInputInt(0, i);
				if (choice == iPlayer) {
					printf("Non puioi scegliere te stesso, riprova\n");
				}
			} while (choice == iPlayer);

			i = 0;
			playerScambio = playerList;
			currCarte     = turno->carteGiocate;

			// Salvo la carta che si ha scelto
			for (int j = 0; j < choice; ++j) {
				currCarte     = currCarte->next;
				playerScambio = playerScambio->nextPlayer;
			}
			pChoosenCard = currCarte;

			// Rifiuto la carta che non ha effetto NESSUNO e chiedo nuovamente l'input
			if (pChoosenCard->effect != NESSUNO){
				printf("Non puoi prendere questa carta, riprova\n");
			}
		} while (pChoosenCard->effect != NESSUNO);


		// Tolgo i punti della carta del giocatore che ha attivato l'effeto e gli assegno i punti della carta scelta
		turno->points[iPlayer] -= cartaPlayer->cfu;
		turno->points[iPlayer] += pChoosenCard->cfu;

		/* tolgo i punti della carta scelta al giocatore che la possedeva e gli assegno quelli della carta del
		 * giocatore che sta scambiando le carte */
		turno->points[choice] -= pChoosenCard->cfu;
		turno->points[choice] += cartaPlayer->cfu;

		// Scambio le due carte

		choosenCardAux = (*pChoosenCard);   // Salvo la carta scekta in una aux
		*pChoosenCard = (*cartaPlayer);     // Salvo la carta del giocatore nella carta scelta
		pChoosenCard->next = choosenCardAux.next;   // Ristabilisco l'ordine di lista
		cartaPlayerAux = (*cartaPlayer);    // Salvo la carta player in aux
		(*cartaPlayer) = choosenCardAux;    // Salvo la carta del giocatore dall'aux
		cartaPlayer->next = cartaPlayerAux.next;    // Ristabilisco l'oridine della lista

		printf("%s e %s si sono scambiati le carte!\n", pPlayer->username, playerScambio->username);
		printf("%s -> %s\n", pPlayer->username, cartaPlayer->name);
		printf("%s -> %s\n", playerScambio->username, pChoosenCard->name);

		// Aggiorno l'array arrRisolte che segna se ho risolto una carta o no
		check = arrRisolte[iPlayer];
		arrRisolte[iPlayer] = arrRisolte[choice];
		arrRisolte[iPlayer] = check;
	}
}


// =====================================================================================================================
/**
 * effettoSCARTAE() subroutine per l'effetto SCARTAE:\n
 * 	Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno
 * @param iPlayer int: indice del giocatore attuale
 * @param pPlayer Player *: giocatore attuale
 * @param mazzoScarti CartaCfu **: mazzo degli scatti
 * @param turno Turno *: struttura del turno
 */
void effettoSCARTAE(int iPlayer, Player *pPlayer,
					CartaCfu **mazzoScarti, Turno *turno) {
	int nonGiocabile = 0,   // Carte non scartabili
		nCarte       = 0;   // Numero di carre
	bool canPlay = false;   // Bool carte in mano
	CartaCfu *discardedCarta = NULL, // Carta scartata
			 *currCarte      = NULL; // Carta corrente della lista

	currCarte = pPlayer->manoCarteCfu; // La lista da scoreere è la mano del giocatore

	// Il giocatore deve avere carte in mano per attivare l'effetto
	if (pPlayer->manoCarteCfu == NULL) {
		canPlay = true;
	}

	// Scorro la lista della mano
	while (currCarte != NULL) {
		if (currCarte->effect == NESSUNO || currCarte->cfu == 0) { // Le carte giocabili sono quelle con effetto
			nonGiocabile++;
		}
		nCarte++; // Conta carte
		currCarte = currCarte->next; // Next carta
	}

	if (nonGiocabile != nCarte && !canPlay) {   // Se si ha almeno una carta giocabile
		discardedCarta = discard(pPlayer);      // Posso selezionare una carta da scartare
		turno->points[iPlayer] += discardedCarta->cfu;  // Aumento i punti della carta scartata
		cartaCfuInCoda(mazzoScarti, discardedCarta); // Aggiungo la carta in coda al manzo scarti
	} else {    // Altrimenti esco dalla subroutine
		printf("Non puoi scartare nessuna carta\n");
	}
}

/**
 * discard() subroutine per scartare una carta
 * @param pPlayer Player *: player da cui scartare la carta
 * @return CartaCfu *: puntatore a carta scartata
 */
CartaCfu *discard(Player *pPlayer) {
	CartaCfu *currCarte   = NULL, // Carta corrente della lista
			 *choosenCard = NULL, // Carta scelta da scartare
			 *prev        = NULL; // Carta precedente

	int choice        = 0, // Indice carta scelta
		count         = 0, // Indice di ogni carta
		nonScartabile = 0; // Carte non scartabili
	bool flag; // BANDIERA ROSSA

	currCarte = pPlayer->manoCarteCfu; // currCarte prende la testa della mano di carte del pPlayer

	// Stampa mano carte
	printf("Scegli una carta con effetto da scartare:\n");
	while (currCarte != NULL) {
		printSingleCartaCfu(currCarte);

		if (currCarte->cfu == 0 || currCarte->effect == NESSUNO) {
			printf("| Non puoi scartare una carta senza punti e effetto\n");
			nonScartabile++;
		}
		currCarte = currCarte->next; // Next carta
	}

	count = contaCarteCfu(currCarte);
	// Se non si hanno carte scartabili, lascio la subroutine
	if (nonScartabile == count){
		printf("Nessuna carta scartabile in questo turno!\n");
		choosenCard = NULL;
	} else { // Altrimenti
		do {
			// Acquisisco scelta
			currCarte = pPlayer->manoCarteCfu;
			choice = acquisisciInputInt(0, count);
			// Controllo che la scelta sia valida
			for (int i = 0; i < choice; ++i) {
				currCarte = currCarte->next;
			}
			if (currCarte->effect == NESSUNO || currCarte->cfu == 0) {
				printf("\nHai scelto una carta che non puoi scartare, riprova\n");
			}
		} while (currCarte->effect == NESSUNO || currCarte->cfu == 0);

		// Trovata una carta ammissibile la posso estrarre
		choosenCard = estraiCartaCfu(&(pPlayer->manoCarteCfu), choosenCard);
	}
	return choosenCard; // Ritorno carta selezionata
}

// ====================================================================================================================
/**
 * effettoSCARTAC() subroutine per l'effetto SCARTAC:\n
 * 	Scarta da uno a tre carte dalla tua mano
 * @param pPlayer Player *:
 * @param mazzoScarti CartaCfu **:
 */
void effettoSCARTAC(Player *pPlayer, CartaCfu **mazzoScarti) {
	//variabili per contare il numero di carte, quante sono state scartate e la scelta della carta
	int numDiscarded = 0, // Carte scartate
	    choice       = 0, // Scelta
		countCarte;

	bool leave           = false; // Flag di uscita ciclo

	CartaCfu *currCarte      = NULL, // Carta corrente della lisra
			 *discardedCarta = NULL; // Carta scartata

	currCarte = pPlayer->manoCarteCfu;
	countCarte = contaCarteCfu(currCarte);
	if (countCarte == 0) {
		printf("Non puoi scartare carte\n");
	}

	do {
		currCarte = pPlayer->manoCarteCfu; // Init currCarte alla mano

		// Stampo carte della mano
		for (int j = 1; j < countCarte; ++j) {
			printf("[%d] ", j);
			printSingleCartaCfu(currCarte);
			currCarte = currCarte->next; // Next carta
		}
		printf("[0] Non scartare altre carte\n");

		currCarte = pPlayer->manoCarteCfu; // Init currCarte alla mano
		choice = acquisisciInputInt(0, countCarte);
		choice--; // Decremento choice perchè j è iniziato da 1

		if (choice == -1) { // Se non si vogliono scartare carte
			printf("Non scarti altre carte\n");
			leave = true; // Lascio il ciclo
		} else {
			discardedCarta = indexEstraiCartaCfu(&currCarte, choice); // Estraggo la carta choice-esima
			*mazzoScarti = cartaCfuInTesta(*mazzoScarti, discardedCarta); // Aggiungo al mazzo scarti
			numDiscarded++; // Incremento contatore carte scartate
		}

		if (numDiscarded == MAX_SCARTABILI){ // Se è stato raggiunto il massimo di carte scartate
			printf("Hai scartato il massimo numero di carte\n");
			leave = true; // Lascio il ciclo
		}
	} while (!leave); // Continuo se leave è false
	printf("Hai scartato %d carte\n", numDiscarded);
}

// ====================================================================================================================
/**
 * effettoSCAMBIAP() subroutine per l'effetto SCAMBIAP\n:
 * 	Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base
 * @param nPlayers int: numero di giocatori
 * @param playerList Player *: lista di giocatori
 * @param turno Turno *: struttura turno
 */
void effettoSCAMBIAP(int nPlayers, Player *playerList, Turno *turno) {
	Player *playerHead = playerList;
	// Se il punteggio di vincita e perdita sono uguali, i giocatori sono tutti pari e non ci sono scambi
	if (turno->cfuToLose != turno->cfuToWin) {
		// Ciclando su tutti i giocatori
		for (int i = 0; i < nPlayers; ++i) {
			// Se ha i punti maggiori, gli assegno i punti minori
			if (turno->points[i] == turno->cfuToWin){
				turno->points[i] = turno->cfuToLose;
				printf("%s ha %d Cfu\n", playerHead->username, turno->cfuToLose);
			} else if (turno->points[i]==turno->cfuToLose) { // Altrimenti se ha i punti minori, gli assegno i maggiori
				turno->points[i] = turno->cfuToWin;
				printf("%s ha %d Cfu\n", playerHead->username, turno->cfuToWin);
			}
			playerHead = playerHead->nextPlayer;
		}
	} else {
		printf("I giocatori sono tutti pari, non ci sono scambi da fare\n");
	}
}

// ====================================================================================================================
/**
 * effettoDOPPIOE() subroutine per l'effetto DOPPIOE:\n
 * 	Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti)
 * @param checkDOPPIOE bool *: puntatore al booleano che indica l'atticazione o meno di quest'effetto
 */
void effettoDOPPIOE(bool *checkDOPPIOE) {
	*checkDOPPIOE = true;
	printf("\n ATTENZIONE!\n"
		   "Da ora in poi gli effetti AUMENTA e DIMINUISCI modificheranno il punteggio di 4 punti\n");
}

// ====================================================================================================================
/**
 * effettoSBIRCIA() subroutine per l'effetto SBIRCIA:\n
 * 	Guarda due carte in cima al mazzo, prendine una e scarta l’altra
 * @param mazzoCfu CartaCfu **: mazzo di pesca Cfu
 * @param pPlayer Player *: player che sbircia
 * @param mazzoScarti CartaCfu **: mazzo degli scarti
 */
void effettoSBIRCIA(CartaCfu **mazzoCfu, Player *pPlayer, CartaCfu **mazzoScarti) {
	CartaCfu *carteSbirciate = NULL, // Carte sbirciate
			 *currCartaCfu   = NULL, // Carta cfu attuale
			 *discardedCarta = NULL; // Carta da scartare

	int choice; // Scelta carta da tenere

	// Se nel mazzo non ci sono abbastanza carte per sbirciare, lo rimescolo
	if (contaCarteCfu(*mazzoCfu) < CARTE_SBIRCIABILI) {
		*mazzoCfu = mescolaMazzo(mazzoScarti);
	}
	// Estraggo una lista delle prime due carte
	carteSbirciate = indexEstraiCartaCfu(mazzoCfu, 0);
	carteSbirciate->next = indexEstraiCartaCfu(mazzoCfu, 0);

	currCartaCfu = carteSbirciate;
	printf("%s sta sbirciando nel mazzo:\n", pPlayer->username);
	// Stampa carte sbirciate
	for (int i = 0; i < CARTE_SBIRCIABILI; ++i) {
		printf("[%d]", i);
		printSingleCartaCfu(currCartaCfu);
		printf("\n");
		currCartaCfu = currCartaCfu->next; // Passaggio alla carta successiva
	}

	// Acquisizione carta da tenere, con controllo errori
	printf("Quale carta vuoi?\n");
	choice = acquisisciInputInt(0, CARTE_SBIRCIABILI - 1);

	// Attauazione scelta carta
	switch (choice) {
		case 0:
			discardedCarta = carteSbirciate->next; // Scarta seconda carta
			discardedCarta->next = NULL;
			carteSbirciate->next = NULL;
			printf("\nStai tenendo %s\n", carteSbirciate->name);
			break;
		case 1:
			discardedCarta = carteSbirciate; // Scarta la prima carta
			carteSbirciate = carteSbirciate->next; // La testa della lista prende la seconda carta
			discardedCarta->next = NULL;
			carteSbirciate->next = NULL;
			printf("Hai scelto %s!\n", carteSbirciate->name);
			break;
	}

	// Aggiungo carte sbirciate alla mano del playere
	cartaCfuInTesta(pPlayer->manoCarteCfu, carteSbirciate);

	// Aggiungo la carta scartata al mazzo di scarti
	cartaCfuInTesta(pPlayer->manoCarteCfu, discardedCarta);
}

// ====================================================================================================================
// TODO CONTROLLAAAAAAAA
/**
 * effettoSCAMBIAC() subroutine per l'effetto SCAMBIAC\n:
 * 	Scambia le carte punto di due giocatori qualsiasi
 * @param carteGiocate
 * @param playerList
 * @param arrRisolte
 */
void effettoSCAMBIAC(CartaCfu **carteGiocate, Player *playerList, bool arrRisolte[], int nPlayers) {
	CartaCfu *currCarte = NULL, // Cursore lista
			 *pCarta1   = NULL, // Carta del giocatore 1
	         *pCarta2   = NULL, // Carta del giocatore 2
	         cartaCfu1, // Strutture per lo scambio delle carte
	         cartaCfu2;

	Player *currPlayer = playerList,
		   *pPlayer1   = NULL,
		   *pPlayer2   = NULL; // Puntatori per giocatori scelti

	bool check = false; // bool di controllo

	int i             = 1, // Index cicli
		choicePlayer1 = 0, // Index carta giocatore 1
		choicePlayer2 = 0; // Index carta giocatore 2

	currCarte = *carteGiocate;

	for (int j = 1; j <= nPlayers; ++j) {
		printf("[%d] Carta di %s:\n", j, currPlayer->username);
		printSingleCartaCfu(currCarte);
		currCarte  = currCarte->next; // Next carta
		currPlayer = currPlayer->nextPlayer; // Next player
	}
	printf("[0] Non effettuare lo scambio\n");

	printf("A chi vuoi scambiare le carte?\n");
	choicePlayer1 = acquisisciInputInt(0, nPlayers + 1);

	if (choicePlayer1 == 0) { // Lascio la subroutine se la scelta è 0
		printf("Nessuno scambio\n");
	} else {
		choicePlayer1--;              // Decremento l'indice perche le opzioni erano incrementate di uno
		currCarte  = (*carteGiocate); // Reinizializzo currCarte alla lista
		currPlayer = playerList;      // currPlayer parte dalla testa della lista dei giocatori

		// Individuo la carta del primo giocatore da scambiare
		do {
			if (i == choicePlayer1) {
				pCarta1 = currCarte;    // Salvo la carta del giocatore scelto
				pPlayer1 = currPlayer;  // Salvo il giocatore a cui verrà scambiata la carta
			} else {                    // Altrimenti continuo al prossimo giocatore e carta
				currPlayer = currPlayer->nextPlayer;
				currCarte  = currCarte->next;
				i++;
			}
		} while (pCarta1 == NULL);  // Ciclo fin quando non viene assegnata la carta

		currCarte = (*carteGiocate);    // Reinizializzo la lista di carte
		i = 0;                          // E l'indice

		// Ciclo per stampare nuovamente giocatori e carte

		printf("Con quale giocatore vuoi scambiare la carta di %s?\n", pPlayer1->username);
		for (int j = 0; j < nPlayers; ++j) {
			printf("[%d] Carta di %s:\n", j, currPlayer->username);
			printSingleCartaCfu(currCarte);
			if (j == choicePlayer1) {
				printf("| CARTA GIÀ SCELTA\n");
			}
			currCarte  = currCarte->next; // Next carta
			currPlayer = currPlayer->nextPlayer; // Next player
		}

		// Acquisisco la scelta dell'utente, controllo che non scelga lo stesso giocatore di prima
		do {
			choicePlayer2 = acquisisciInputInt(0, nPlayers);

			if (choicePlayer2 == choicePlayer1) {
				printf("\nHai scelto due volte lo stesso giocatore, riprova\n");
			}
		} while (choicePlayer2 == choicePlayer1);


		i         = 0; // Reset indice
		currCarte = (*carteGiocate); // Reset lista carte
		currPlayer  = playerList; // Reset lista giuocatori

		// Individuo la carta del secondo giocatore da scambiare
		do {
			if (i == choicePlayer2) {
				pCarta2 = currCarte;    // Salvo la carta del giocatore scelto
				pPlayer2 = currPlayer;  // Salvo il giocatore a cui verrà scambiata la carta
			} else {                    // Altrimenti continuo al prossimo giocatore e carta
				currPlayer = currPlayer->nextPlayer;
				currCarte  = currCarte->next;
				i++;
			}
		} while (pCarta2 == NULL);  // Ciclo fin quando non viene assegnata la carta

		/*
		 * Poichè abbiamo una relazione biunivoca tra giocatroe e carta nella lista di carte giocate, è più comodo
		 * scambiare i contenuti dei nodi, piuttosto che i nodi in se e per se
		 */

		// TODO CONTROLLA
		cartaCfu2 = (*pCarta2); // salvo il contenuto della carta giocatore 2 nella struttura ausiliaria
		*pCarta2 = (*pCarta1);  // assegno il contenuto della carta del giocatore 1 al giocatore 2
								// ristabilisco l'ordine della lista usando la carta ausiliaria
		pCarta2->next = cartaCfu2.next;
		cartaCfu1 = (*pCarta1); //salvo il contenuto della carta giocatore 1 nella struttura ausiliaria
		(*pCarta1) = cartaCfu2; //assegno il contenuto della carta del giocatore 2 al giocatore 1
		//ristabilisco l'ordine della lista usando la carta ausiliaria
		pCarta1->next = cartaCfu1.next;
		printf("%s e %s si sono scambiati le carte!\n"
		       "%s <- Carta: %s\n"
			   "%s <- Carta: %s",
		       pPlayer1->username, pPlayer2->username,
		       pPlayer1->username, pCarta1->name,
			   pPlayer2->username, pCarta2->name);
		/*arrRisolte[] tiene conto delle carte che hanno risolto l'effetto con corrispondenza tra indice dell'array e
		 * posizione della carta nella lista, questo vuol dire che se due carte vengono scambiate devo scambiare anche
		 * il controllo delle risoluzioni delle carte */
		check = arrRisolte[choicePlayer1];
		arrRisolte[choicePlayer1] = arrRisolte[choicePlayer2];
		arrRisolte[choicePlayer2] = check;
	}
}

// TODO ¡¡¡CONTROLLA QUESTE SUBROUTINE¡¡¡