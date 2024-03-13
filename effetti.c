//
// Created by Monti on 26/02/24.
//

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
void gestioneEffetti(int nPlayers, Player *playerList, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti, Turno *turno,
                     bool *checkDOPPIOE) {
	bool *arrRisolte    = NULL;     // Array controllo risoluzione carta
	int cfuMax          = INT_MIN;  // Cfu maggiori tra le carte giocate
	Player *currPlayer  = NULL;     // Puntatore al giocatore corrente
	CartaCfu *currCarta = NULL;     // Puntatore alla carta corrente

	arrRisolte = allocaArrBool(nPlayers); // Alloco l'array di controllo risoluzione carte
	currPlayer = playerList;    // Init di currPlayer a playerList

	for (int j = 0; j < nPlayers; ++j) {
		currCarta = turno->carteGiocate;
		cfuMax    = INT_MIN;
		for (int i = 0; i < nPlayers; i++) {
			if (!arrRisolte[i] && currCarta->cfu > cfuMax) {
				cfuMax = currCarta->cfu;
			}
			currCarta = currCarta->next;
		}

		currCarta  = turno->carteGiocate;
		currPlayer = playerList;

		for (int i = 0; i < nPlayers; ++i) {
			if (!arrRisolte[i] && currCarta->cfu == cfuMax) {
				if (currCarta->effect != ANNULLA) {
					arrRisolte[i] = risolviEffetti(i, currPlayer, nPlayers, playerList,
					                               currCarta, mazzoCfu, mazzoScarti,
					                               turno,
					                               arrRisolte, checkDOPPIOE);
					enterClear();
				} else {
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
			effettoSCARTAP(mazzoScarti, currPlayer, iPlayer, turno);
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
			effettoSCAMBIAP(turno, nPlayers, playerList);
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
			break;
	}
	return true;
}

// ====================================================================================================================
/**
 * effettoSCARTAP subroutine per l'effetto SCARTAP: \n
 * 	Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno \n
 * @param mazzoScarti CartaCfu **: mazzo degli scarti
 * @param pPlayer Player *: giocatore a cui appartiene la carta
 * @param iPlayer int: indice giocatore
 * @param turno Turno *: struttura turno
 */
void effettoSCARTAP(CartaCfu **mazzoScarti, Player *pPlayer, int iPlayer, Turno *turno) {
	int nonGiocabile      = 0,        // Numero carte non giocabili
		numCarte          = 0;            // Numero carte totali
	bool flag             = false;           // controllo
	CartaCfu *choosenCard = NULL,   // carta scelta
			 *currMano    = NULL;   // puntatore per scorrere la lista di carte in mano

	currMano = pPlayer->manoCarteCfu;

	if(pPlayer->manoCarteCfu==NULL) { // se la mano è vuota la variabile è settata true
		flag = true;
	}

	while (currMano != NULL) {
		if (isIstantanea(currMano)) { // In questa fase non sono ammesse carte istantanee
			nonGiocabile++;           // incremento le carte non giocabili
		}
		numCarte++; // conto il numero di carte
		currMano = currMano->next; // passaggio alla carta successiva
	}

	currMano = pPlayer->manoCarteCfu;

	if (nonGiocabile != numCarte && flag != false) { // se c'è almeno una carta giocabile
		choosenCard = chooseCarta(&currMano, NULL, mazzoScarti, SPAREGGIO);
		choosenCard = estraiCartaCfu(&currMano, choosenCard);
		turno->points[iPlayer] += choosenCard->cfu;  //assegno i punti della carta al punteggio del giocatore
		printf("%s aggiungi %d CFU al tuo punteggio parziale!\n", pPlayer->username, choosenCard->cfu);
		//la carta viene scartata
		choosenCard->next = (*mazzoScarti);
		(*mazzoScarti) = choosenCard;
	} else { //se non ci sono carte giocabili
		printf("Nessuna carta scartabile in questo turno! Potrebbe essere un bel problema...\n");
	}
}

// ====================================================================================================================
/**
 * effettoRUBA() subroutine per effetto RUBA:\n
 * 	Guarda la mano di un collega e ruba una carta a scelta
 * @param playerList Player *: lista dei giocatori
 * @param pPlayer Player *: player a cui appartiene la carta
 */
void effettoRUBA(Player *playerList, Player *pPlayer) {
	Player *currPlayer   = NULL;
	CartaCfu *stolenCard = NULL;
	int i      = 0,
		choice = 0;
	bool reTry = false;
	currPlayer = playerList;

	printf("\nA chi vuoi rubare una carta?\n");
	while (currPlayer != NULL){
		if (strcmp(currPlayer->username, pPlayer->username) != 0) {
			printf("[%d] %s\n", i, currPlayer->username);
		}
		i++;
	}

	currPlayer = playerList;
	do {
		reTry = false;
		printf("\n>>> ");
		scanf("%d", &choice);

		if (choice < 0 || choice > i) {
			printf("\nIl valore inserito non è valido, riprova\n");
			reTry = true;
		} else {
			for (int j = 0; j < choice; ++j) {
				currPlayer = currPlayer->nextPlayer;
			}
			if (strcmp(currPlayer->username, pPlayer->username) == 0) {
				printf("\nIl valore inserito non è valido, riprova\n");
				reTry = true;
			}
		}
	} while (reTry);

	currPlayer = playerList;
	for (int j = 0; j < choice; ++j) {
		currPlayer = currPlayer->nextPlayer;
	}

	stolenCard = steal(currPlayer);
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
	int      i           = 0,
	         choice      = 0;

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

// ====================================================================================================================

/**
 * effettoSCAMBIADS() subroutine per effetto SCAMBIADS:\n
 * 	Scambia questa carta con quella di un altro giocatore, purché senza effetto
 * @param iPlayer int:
 * @param pPlayer Player *:
 * @param nPlayers int:
 * @param playerList Player *:
 * @param pCarta CartaCfu *:
 * @param turno Turno *:
 * @param arrRisolte bool *:
 */
void effettoSCAMBIADS(int iPlayer, Player *pPlayer,
					  int nPlayers, Player *playerList,
					  CartaCfu *pCarta,
					  Turno *turno, bool arrRisolte[]) {
	Player *playerScambio = NULL; // puntatore in cui salvare il giocatore con cui si effettua lo scambio

	CartaCfu *currCarte    = NULL, // variabile ausiliaria per scorrere la lista delle carte giocate
			 *pCartaPlayer = NULL, // puntatore per salvare la carta del giocatore
			 *pChoosenCard = NULL, // puntatore per salvare la carta scelta dal giocatore
			 choosenCardAux,
			 cartaPlayerAux;
	bool check;

	int i = 0, // i: contatore per i cicli do-while
		choice = 0, // choice: variabile in cui inserire l'indice della carta scelta
		noScambio = 0; // noScambia: contatore carte con cui non è possibile effettuare lo scambio

	currCarte = turno->carteGiocate;

	do { // ciclo per trovare la carta del giocatore
		if (i == iPlayer){ // se l'indice del ciclo e quello del giocatore coincidono salvo la carta del giocatore
			pCartaPlayer = currCarte;
		}
		if (currCarte == NULL){ // controllo, se non trova la carta giocata e le carte sono terminate esco per evitare errori
			printf("Carta del giocatore non trovata, uscita con errore!");
			exit(EXIT_FAILURE);
		}
		currCarte = currCarte->next;
		i++;
	} while (pCartaPlayer == NULL);

	currCarte = turno->carteGiocate; // assegno la lista delle carte giocate
	i         = 0;

	printf("\nCarte giocate questo turno:");
	do {
		if (i != iPlayer) {
			printf("[%d] Carta di %s:\n", i, pPlayer->username);
			printSingleCartaCfu(currCarte);
			if (currCarte->effect != NESSUNO) { // se la carta  ha effetti non può essere scambiata
				printf("| Questa carta ha un effetto, quindi non puoi prenderla, mi dispiace\n");
				noScambio++; // incremento carte non scambiabili
			}
		} else {
			noScambio++; // incremento le carte non scambiabili
		}
		i++;
		currCarte  = currCarte->next;
		playerList = playerList->nextPlayer;
	} while (currCarte != NULL);

	if (i == noScambio) { // se il numero di carte coincide con il numero di carte non scambiabili termina la funzione
		printf("Nessuna carta con cui scambiare la tua!\n");
	} else {
		do {
			do {
				printf("\n>>> ");
				scanf("%d", &choice);
				if(choice < 0 || choice > i|| choice == iPlayer) {
					printf("Errore, riprovare\n");
				}
			} while (choice < 0 || choice > i-1 || choice == iPlayer);

			i = 0;
			playerScambio = playerList;
			currCarte     = turno->carteGiocate;

			do {
				if (i == choice) {
					pChoosenCard = currCarte;
				} else {
					i++;
					currCarte     = currCarte->next;
					playerScambio = playerScambio->nextPlayer;
				}
			} while (pChoosenCard == NULL);

			if (pChoosenCard->effect != NESSUNO){ // se la carta non poteva essere presa stampa un errore
				printf("Non puoi prendere questa carta, riprova\n");
			}
		} while (pChoosenCard->effect != NESSUNO); //se la carta non poteva essere presa ripeto la scelta


		/* tolgo i punti della carta del giocatore che sta facendo la scelta e gli assegno i punti
		 * della carta che ha scelto */
		turno->points[iPlayer] -= pCartaPlayer->cfu;
		turno->points[iPlayer] += pChoosenCard->cfu;

		/* tolgo i punti della carta scelta al giocatore che possedeva quella carta e gli
		* assegno i punti della carta del giocatore che sta scambiando le carte */

		turno->points[choice] -= pChoosenCard->cfu;
		turno->points[choice] += pCartaPlayer->cfu;

		choosenCardAux = (*pChoosenCard); // assegno il contenuto della carta a una struttura ausiliaria
		// scambio le informazioni tra la carta giocatore e la carta scelta
		*pChoosenCard = (*pCartaPlayer);
		// assegno il successivo della carta ausiliaria in modo da ristabilire l'ordine della lista
		pChoosenCard->next = choosenCardAux.next;
		//assegno il contenuto della carta del giocatore a una carta
		cartaPlayerAux = (*pCartaPlayer);
		(*pCartaPlayer) = choosenCardAux; //assegno il contenuto della carta alla carta del giocatore
		//assegnazione del successivo della cat per ristabilire l'ordine della lista
		pCartaPlayer->next = cartaPlayerAux.next;

		printf("%s e %s si sono scambiati le carte!\n", pPlayer->username, playerScambio->username);
		printf("%s prende carta: %s\n", pPlayer->username, pCartaPlayer->name);
		printf("%s -> Carta: %s\n", playerScambio->username, pChoosenCard->name);

		/*effetti[] tiene conto delle carte che hanno risolto l'effetto con corrispondenza tra indice dell'array e
		 * posizione della carta nella lista, questo vuol dire che se due carte vengono scambiate devo scambiare anche
		 * il controllo delle risoluzioni delle carte */
		check = arrRisolte[iPlayer];
		arrRisolte[iPlayer] = arrRisolte[choice];
		arrRisolte[iPlayer] = check;
	}
}


// ====================================================================================================================
/**
 * effettoSCARTAE() subroutine per l'effetto SCARTAE:\n
 * 	Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno
 * @param iPlayer int:
 * @param pPlayer Player *:
 * @param mazzoScarti CartaCfu **:
 * @param turno Turno *:
 */
void effettoSCARTAE(int iPlayer, Player *pPlayer,
					CartaCfu **mazzoScarti, Turno *turno) {
	//variabili contatore per il numero di carte e il numero di carte non scartabili
	int nonGiocabile    = 0, nCarte = 0;
	bool flag           = false; // bool di controllo
	CartaCfu *cartaCfu  = NULL, // puntatore in cui viene salvata la carta scartata
			 *currCarte = NULL; // puntatore ausiliario per scorrere la lista
	currCarte = pPlayer->manoCarteCfu; // assegnazione della mano al puntatore ausiliario

	if (pPlayer->manoCarteCfu == NULL) {
		flag = true; //se il giocatore non ha carte in mano flag va a true
	}

	while (currCarte != NULL) { //ciclo che continua fino alla fine della lista delle carte in mano
		if (currCarte->effect == NESSUNO || currCarte->cfu == 0) { //controllo se non sono giocabili
			nonGiocabile++;
		}
		nCarte++; //conteggio carte
		currCarte = currCarte->next; // passaggio alla prossima carta
	}

	if (nonGiocabile != nCarte && !flag) { // se c'è almeno una carta ed è giocabile
		cartaCfu = discard(pPlayer); // scelta della carta attraverso subroutine sceltaCarta()
		turno->points[iPlayer] += cartaCfu->cfu; // aggiungo i punti della carta scartata
		cartaCfuInCoda(mazzoScarti, cartaCfu);
	} else { // se non ci sono carte giocabili esco dalla subroutine senza fare nulla
		printf("Nessuna carta scartabile in questo turno!\n");
	}
}

/**
 * discard() subroutine per scartare una carta
 * @param pPlayer Player *:
 * @return
 */
CartaCfu *discard(Player *pPlayer) {
	CartaCfu *currCarte   = NULL,
			 *choosenCard = NULL,
			 *prev        = NULL;
	/* choice: indice carta scelta dal pPlayer
	 * index: indice di ogni carta (inizializzato a -1 in modo che il conteggio parta da 0)
	 */
	int choice            = 0, // choice: indice carta scelta dal pPlayer
		index             = 0, // index: indice di ogni carta (inizializzato a -1 in modo che il conteggio parta da 0)
		nonScartabile     = -1;
	bool flag; // booleano di controllo

	currCarte = pPlayer->manoCarteCfu; //currCarte prende la testa della mano di carte del pPlayer
	printf("Scegli una carta da scartare, ricorda: solo carte punto con effetto:\n");
	do {
		if (currCarte->cfu != 0 && currCarte->effect != NESSUNO) {
			printSingleCartaCfu(currCarte);
			printf("\n");
		} else {
			printSingleCartaCfu(currCarte);
			printf("| Carta senza effetto o senza punti: Non puoi scartare questa carta");
			printf("\n");
			nonScartabile++;
		}
		index++;
		currCarte = currCarte->next; // il puntatore va alla prossima carta
	} while (currCarte != NULL); // continua fino all'ultima carta

	if (nonScartabile == index){
		printf("Nessuna carta scartabile in questo turno!\n");
		choosenCard = NULL;
	} else {

		do {
			flag      = false;
			currCarte = pPlayer->manoCarteCfu;
			printf(">>> ");
			scanf("%d", &choice); //acquisizione scelta carta

			if (choice < 0 || choice > index) {
				printf("\nInput non valido, riprovare\n");
				flag = true;
			} else {
				for (int i = 0; i < index; ++i) {
					currCarte = currCarte->next;
				}
				if (currCarte->effect == NESSUNO || currCarte->cfu == 0) {
					printf("\nHai scelto una carta che non puoi scartare, riprova\n");
					flag = true;
				}
			}
		} while (flag);

		if (choice == 0) {
			choosenCard = pPlayer->manoCarteCfu; // la carta prende la testa della lista
			pPlayer->manoCarteCfu = pPlayer->manoCarteCfu->next; // cambio della testa della lista
			choosenCard->next = NULL; // la carta successiva a quella scelta prende NULL come indirizzo
		} else {
			// ciclo for per raggiungere la carta precedente a quella scelta tramite puntatore currCarte
			for (int i  = 0; i < choice; ++i) {
				prev      = currCarte;
				currCarte = currCarte->next;
			}
			choosenCard = currCarte; // puntatore alla carta scelta dall'utente
			prev->next      = currCarte->next; // la lista viene generata senza la carta
			currCarte->next = NULL; // la carta successiva a quella scelta prende NULL come indirizzo
		}
	}
	return choosenCard; // ritorno della carta selezionata
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
	int nDiscardedCarte = 0,
	    choice          = 0,
	    i               = 0;

	bool stop = false; //bool di controllo per fermare il ciclo per scartare

	CartaCfu *currCarte      = NULL, //puntatore ausiliario per scorrere la lista
			 *discardedCarta = NULL, //puntatore dove salvare la carta scartata
			 *prev           = NULL;

	do { //ciclo per ripetere lo scartare la carta
		currCarte = pPlayer->manoCarteCfu; //assegnazione lista delle carte ad currCarte
		if (currCarte == NULL) { //se non ci sono carte
			printf("Non hai carte da scartare!\n");
			stop = true; // blocco del ciclo
		} else {
			while (currCarte != NULL) { //controlla tutte le carte
				printf("[%d] ", i + 1);
				printSingleCartaCfu(currCarte);
				currCarte = currCarte->next; //passaggio alla prossima carta
				i++;
			}
			printf("[0] Non scartare nulla\n");

			choice = acquisisciInputInt(0, i);
			choice--;

			if (choice == -1) { // se non si vogliono scartare carte
				printf("Hai scelto di non scartare carte\n");
				stop = true; //blocco il ciclo

			} else if (choice == 0) { //eliminazione carta in testa
				discardedCarta = pPlayer->manoCarteCfu; // selezione carta scartata
				pPlayer->manoCarteCfu  = discardedCarta->next; // cambio testa della lista
				// inserimento in testa nella pila degli scarti
				cartaCfuInCoda(mazzoScarti, discardedCarta);
				nDiscardedCarte++; // incremento carte scartate

			} else { //scarta carta in mezzo alla lista o in coda
				currCarte = pPlayer->manoCarteCfu; // assegnazione lista carte ad currCarte
				//ciclo per arrivare alla carta precedente di quella scelta
				for (int j = 0; j < choice; ++j) {
					prev      = currCarte;
					currCarte = currCarte->next; //passaggio alla carta successiva
				}
				discardedCarta = currCarte; //la carta scartata è la successiva ad currCarte
				prev->next = discardedCarta->next; //rimuovo la carta dalla lista delle carte in mano

				// inserimento in testa nella pila degli scarti
				cartaCfuInCoda(mazzoScarti, discardedCarta);
				nDiscardedCarte++; //incremento il numero di carte scartate
			}
		}
		if (nDiscardedCarte == MAX_SCARTABILI){ // se ho raggiunto il massimo di carte scartate
			stop = true; // blocco il ciclo
		}
	} while (!stop); // il ciclo continua finché la variabile di controllo è a false

	if (nDiscardedCarte > 0) { //se ho scartato almeno una carta
		printf("Hai scartato %d carte\n", nDiscardedCarte);
	} else {
		printf("Non hai scartato nessuna carta\n");
	}
}

// ====================================================================================================================
/**
 * effettoSCAMBIAP() subroutine per l'effetto SCAMBIAP\n:
 * 	Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base
 * @param turno
 * @param nPlayers
 * @param playerList
 */
void effettoSCAMBIAP(Turno *turno, int nPlayers, Player *playerList) {
	Player *playerHead = playerList;
	if (turno->cfuToLose != turno->cfuToWin) {
		for (int i = 0; i < nPlayers; ++i) {
			if (turno->points[i] == turno->cfuToWin){
				turno->points[i] = turno->cfuToLose;
				printf("%s passa al punteggio minimo! [%d CFU]\n", playerHead->username, turno->cfuToLose);
			} else if (turno->points[i]==turno->cfuToLose){
				turno->points[i] = turno->cfuToWin;
				printf("%s passa al punteggio massimo!  [%d CFU]\n", playerHead->username, turno->cfuToWin);
			}
			playerHead = playerHead->nextPlayer;
		}
	} else {
		printf("I giocatori al momento sono tutti pari! Nessuno scambio\n");
	}
}

// ====================================================================================================================
/**
 * effettoDOPPIOE() subroutine per l'effetto DOPPIOE:\n
 * 	Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti)
 * @param checkDOPPIOE
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

	if (contaCarteCfu(*mazzoCfu) <= CARTE_SBIRCIABILI) {
		*mazzoCfu = mescolaMazzo(mazzoScarti);
	}
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