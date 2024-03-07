//
// Created by Monti on 26/02/24.
//

#include "effetti.h"

//typedef enum {
//	NESSUNO, 	/**< Carta senza effetto */
//	SCARTAP, 	/**< Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno */
//	RUBA, 		/**< Guarda la mano di un collega e ruba una carta a scelta */
//	SCAMBIADS, 	/**< Scambia questa carta con quella di un altro giocatore, purché senza effetto */
//	SCARTAE, 	/**< Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno */
//	SCARTAC, 	/**< Scarta da uno a tre carte dalla tua mano */
//	SCAMBIAP, 	/**< Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base */
//	DOPPIOE, 	/**< Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti) */
//	SBIRCIA, 	/**< Guarda due carte in cima al mazzo, prendine una e scarta l’altra */
//	SCAMBIAC,   /**< Scambia le carte punto di due giocatori qualsiasi */
//	ANNULLA, 	/**< Annulla gli effetti di tutte le carte punto durante il turno */
//	AUMENTA, 	/**< Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta */
//	DIMINUISCI, /**< Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta */
//	INVERTI, 	/**< Inverti punteggio minimo e massimo del turno */
//	SALVA, 		/**< Metti la carta Ostacolo che stai per prendere in fondo al mazzo */
//	DIROTTA 	/**< Dai la carta che stai per prendere ad un altro giocatore a tua scelta */
//} Effect;

void gestioneEffetti(int nPlayers, Player *playerList, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti, Turno *turno,
                     bool *checkDOPPIOE) {
	bool *arrRisolte = NULL;
	int cfuMax = INT_MIN;
	Player *pPlayer = NULL;
	pPlayer = playerList;
	CartaCfu *pCarta = NULL;
	arrRisolte = allocaArrBool(nPlayers);

	for (int j = 0; j < nPlayers; ++j) {
		pCarta = turno->carteGiocate;
		cfuMax = INT_MIN;
		for (int i = 0; i < nPlayers; i++) {
			if (!arrRisolte[i] && pCarta->cfu > cfuMax) {
				cfuMax = pCarta->cfu;
			}
			pCarta = pCarta->next;
		}

		pCarta = turno->carteGiocate;
		pPlayer = playerList;

		for (int i = 0; i < nPlayers; ++i) {
			if (!arrRisolte[i] && pCarta->cfu == cfuMax) {
				if (pCarta->effect != ANNULLA) {
					arrRisolte[i] = risolviEffetti(i, pPlayer, nPlayers, playerList,
												   pCarta, mazzoCfu, mazzoScarti,
					                               turno,
												   arrRisolte, checkDOPPIOE);
					enterClear();
				} else {
					printf("La carta %s di %s ha annullato tutti gli altri effetti delle carte\n",
					       pCarta->name, pPlayer->username);
					i = nPlayers;
					j = nPlayers;
					enterClear();
				}
			}
			pCarta  = pCarta->next;
			pPlayer = pPlayer->nextPlayer;
		}
	}
}

bool risolviEffetti(int iPlayer, Player *pPlayer, int nPlayers, Player *playerList,
					CartaCfu *pCarta, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
					Turno *turno,
					bool arrRisolte[], bool *checkDOPPIOE) {
	printf("Risolvo effetto della carta %s di %s\n", pPlayer->username, pCarta->name);

	switch (pCarta->effect) {
		case SCARTAP:
			effettoSCARTAP(mazzoScarti, pPlayer, turno, iPlayer);
			break;
		case RUBA:
			effettoRUBA(playerList, pPlayer);
			break;
		case SCAMBIADS:
			effettoSCAMBIADS(playerList, pPlayer, turno, iPlayer, nPlayers, pCarta, arrRisolte);
			break;
		case SCARTAE:
			effettoSCARTAE(mazzoScarti, pPlayer, turno, iPlayer);
			break;
		case SCARTAC:
			effettoSCARTAC(pPlayer, mazzoScarti);
			break;
		case SCAMBIAP:
			effettoSCAMBIAP(turno, nPlayers, playerList);
			break;
		case DOPPIOE:
			effettoDOPPIOE(checkDOPPIOE)
			break;
		case SBIRCIA:
			effettoSBIRCIA(mazzoCfu, pPlayer, mazzoScarti);
			break;
		case SCAMBIAC:
			effettoSCAMBIAC(&(turno->carteGiocate), playerList, arrRisolte);
			break;
		default:
			;
		break;
	}
	return true;
}

/**
 * effettoSCARTAP è la subroutine per l'effetto SCARTAP: \n
 * 	Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno \n
 *
 */
void effettoSCARTAP(CartaCfu **mazzoScarti, Player *pPlayer, Turno *turno, int iPlayer) {
	int nonGiocabile = 0,        // Numero carte non giocabili
		numCarte = 0;            // Numero carte totali
	bool flag = false;           // controllo
	CartaCfu *choosenCard = NULL,   // carta scelta
			 *headMano    = NULL;   // puntatore per scorrere la lista di carte in mano

	headMano = pPlayer->manoCarteCfu;

	if(pPlayer->manoCarteCfu==NULL) { // se la mano è vuota la variabile è settata true
		flag = true;
	}

	while (headMano != NULL) {
		if (isIstantanea(headMano)) { // In questa fase non sono ammesse carte istantanee
			nonGiocabile++;           // incremento le carte non giocabili
		}
		numCarte++; // conto il numero di carte
		headMano = headMano->next; // passaggio alla carta successiva
	}

	headMano = pPlayer->manoCarteCfu;

	if (nonGiocabile != numCarte && flag != false) { // se c'è almeno una carta giocabile
		choosenCard = chooseCarta(&headMano, mazzoScarti, NULL, SPAREGGIO);
		choosenCard = estraiCartaCfu(&headMano, choosenCard);
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

void effettoRUBA(Player *playerList, Player *pPlayer) {
	Player *playerHead = NULL;
	CartaCfu *stolenCard = NULL;
	int i      = 0,
		choice = 0;
	bool reTry = false;
	playerHead = playerList;

	printf("\nA chi vuoi rubare una carta?\n");
	while (playerHead != NULL){
		if (strcmp(playerHead->username, pPlayer->username) != 0) {
			printf("[%d] %s\n", i, playerHead->username);
		}
		i++;
	}

	playerHead = playerList;
	do {
		reTry = false;
		printf("\n>>> ");
		scanf("%d", &choice);

		if (choice < 0 || choice > i) {
			printf("\nIl valore inserito non è valido, riprova\n");
			reTry = true;
		} else {
			for (int j = 0; j < choice; ++j) {
				playerHead = playerHead->nextPlayer;
			}
			if (strcmp(playerHead->username, pPlayer->username) == 0) {
				printf("\nIl valore inserito non è valido, riprova\n");
				reTry = true;
			}
		}
	} while (reTry);

	playerHead = playerList;
	for (int j = 0; j < choice; ++j) {
		playerHead = playerHead->nextPlayer;
	}

	stolenCard = steal(playerHead);
	cartaCfuInCoda(&(pPlayer->manoCarteCfu), stolenCard);

}

CartaCfu *steal(Player *pPlayer) {
	CartaCfu *head = pPlayer->manoCarteCfu,
			 *prev = NULL,
			 *stolenCard = NULL;
	int      i      = 0,
	         choice = 0;

	// Mostra carte che si possono rubare
	printf("\nQuale carta vuoi rubare a %s?\n", pPlayer->username);
	while (head != NULL) {
		printf("[%d] ", i);
		printSingleCartaCfu(head);
		head = head->next;
		i++;
	}

	// Scelta della carta da rubare
	do {
		printf("\n>>> ");
		scanf("%d", &choice);

		if (choice < 0 || choice > i) {
			printf("\nInput non valido, riprovare");
		}
	} while (choice < 0 || choice > i);

	// Actually rubando la carta
	head = pPlayer->manoCarteCfu;

	if (choice == 0) {
		stolenCard = pPlayer->manoCarteCfu;
		pPlayer->manoCarteCfu = pPlayer->manoCarteCfu->next;
	} else {
		for (int j = 0; j < choice; ++j) {
			prev = head;
			head = head->next;
		}
		stolenCard = head;
		prev->next = head->next;
		head->next = NULL;
	}

	return stolenCard;
}

// ====================================================================================================================

void effettoSCAMBIADS(Player *playerList, Player *pPlayer, Turno *turno, int iPlayer, int nPlayers, CartaCfu *pCarta,
					  bool arrRisolte[]) {
	Player *playerScambio = NULL; // puntatore in cui salvare il giocatore con cui si effettua lo scambio

	CartaCfu *headCarte    = NULL, // variabile ausiliaria per scorrere la lista delle carte giocate
			 *pCartaPlayer = NULL, // puntatore per salvare la carta del giocatore
			 *pChoosenCard = NULL, // puntatore per salvare la carta scelta dal giocatore
			 choosenCardAux,
			 cartaPlayerAux;
	bool check;

	int i = 0, // i: contatore per i cicli do-while
		choice = 0, // choice: variabile in cui inserire l'indice della carta scelta
		noScambio = 0; // noScambia: contatore carte con cui non è possibile effettuare lo scambio

	headCarte = turno->carteGiocate;

	do { // ciclo per trovare la carta del giocatore
		if (i == iPlayer){ // se l'indice del ciclo e quello del giocatore coincidono salvo la carta del giocatore
			pCartaPlayer = headCarte;
		}
		if (headCarte == NULL){ // controllo, se non trova la carta giocata e le carte sono terminate esco per evitare errori
			printf("Carta del giocatore non trovata, uscita con errore!");
			exit(EXIT_FAILURE);
		}
		headCarte = headCarte->next;
		i++;
	} while (pCartaPlayer == NULL);

	headCarte = turno->carteGiocate; // assegno la lista delle carte giocate
	i = 0;

	printf("\nCarte giocate questo turno:");
	do {
		if (i != iPlayer) {
			printf("[%d] Carta di %s:\n", i, pPlayer->username);
			printSingleCartaCfu(headCarte);
			if (headCarte->effect != NESSUNO) { // se la carta  ha effetti non può essere scambiata
				printf("| Questa carta ha un effetto, quindi non puoi prenderla, mi dispiace\n");
				noScambio++; // incremento carte non scambiabili
			}
		} else {
			noScambio++; // incremento le carte non scambiabili
		}
		i++;
		headCarte = headCarte->next;
		playerList = playerList->nextPlayer;
	} while (headCarte != NULL);

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
			headCarte = turno->carteGiocate;

			do {
				if (i == choice) {
					pChoosenCard = headCarte;
				} else {
					i++;
					headCarte = headCarte->next;
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

void effettoSCARTAE(CartaCfu **mazzoScarti, Player *pPlayer, Turno *turno, int iPlayer) {
	//variabili contatore per il numero di carte e il numero di carte non scartabili
	int nonGiocabile    = 0, nCarte = 0;
	bool flag           = false; // bool di controllo
	CartaCfu *cartaCfu  = NULL, // puntatore in cui viene salvata la carta scartata
			 *headCarte = NULL; // puntatore ausiliario per scorrere la lista
	headCarte = pPlayer->manoCarteCfu; // assegnazione della mano al puntatore ausiliario

	if (pPlayer->manoCarteCfu == NULL) {
		flag = true; //se il giocatore non ha carte in mano flag va a true
	}

	while (headCarte != NULL) { //ciclo che continua fino alla fine della lista delle carte in mano
		if (headCarte->effect == NESSUNO || headCarte->cfu == 0) { //controllo se non sono giocabili
			nonGiocabile++;
		}
		nCarte++; //conteggio carte
		headCarte = headCarte->next; // passaggio alla prossima carta
	}

	if (nonGiocabile != nCarte && !flag) { // se c'è almeno una carta ed è giocabile
		cartaCfu = discard(pPlayer); // scelta della carta attraverso subroutine sceltaCarta()
		turno->points[iPlayer] += cartaCfu->cfu; // aggiungo i punti della carta scartata
		cartaCfuInCoda(mazzoScarti, cartaCfu);
	} else { // se non ci sono carte giocabili esco dalla subroutine senza fare nulla
		printf("Nessuna carta scartabile in questo turno!\n");
	}
}

CartaCfu *discard(Player *pPlayer) {
	CartaCfu *headCarte   = NULL,
			 *choosenCard = NULL,
			 *prev        = NULL;
	/* choice: indice carta scelta dal pPlayer
	 * index: indice di ogni carta (inizializzato a -1 in modo che il conteggio parta da 0)
	 */
	int choice = 0, // choice: indice carta scelta dal pPlayer
		index  = 0, // index: indice di ogni carta (inizializzato a -1 in modo che il conteggio parta da 0)
		nonScartabile = -1;
	bool flag; // booleano di controllo

	headCarte = pPlayer->manoCarteCfu; //headCarte prende la testa della mano di carte del pPlayer
	printf("Scegli una carta da scartare, ricorda: solo carte punto con effetto:\n");
	do {
		if (headCarte->cfu != 0 && headCarte->effect != NESSUNO) {
			printSingleCartaCfu(headCarte);
			printf("\n");
		} else {
			printSingleCartaCfu(headCarte);
			printf("| Carta senza effetto o senza punti: Non puoi scartare questa carta");
			printf("\n");
			nonScartabile++;
		}
		index++;
		headCarte = headCarte->next; // il puntatore va alla prossima carta
	} while (headCarte != NULL); // continua fino all'ultima carta

	if (nonScartabile == index){
		printf("Nessuna carta scartabile in questo turno!\n");
		choosenCard = NULL;
	} else {

		do {
			flag = false;
			headCarte = pPlayer->manoCarteCfu;
			printf(">>> ");
			scanf("%d", &choice); //acquisizione scelta carta

			if (choice < 0 || choice > index) {
				printf("\nInput non valido, riprovare\n");
				flag = true;
			} else {
				for (int i = 0; i < index; ++i) {
					headCarte = headCarte->next;
				}
				if (headCarte->effect == NESSUNO || headCarte->cfu == 0) {
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
			// ciclo for per raggiungere la carta precedente a quella scelta tramite puntatore headCarte
			for (int i  = 0; i < choice; ++i) {
				prev = headCarte;
				headCarte = headCarte->next;
			}
			choosenCard = headCarte; // puntatore alla carta scelta dall'utente
			prev->next   = headCarte->next; // la lista viene generata senza la carta
			headCarte->next = NULL; // la carta successiva a quella scelta prende NULL come indirizzo
		}
	}
	return choosenCard; // ritorno della carta selezionata
}

// ====================================================================================================================

void effettoSCARTAC(Player *pPlayer, CartaCfu **mazzoScarti) {
	//variabili per contare il numero di carte, quante sono state scartate e la scelta della carta
	int nDiscardedCarte = 0,
	    choice          = 0,
	    i               = 0;

	bool stop = false; //bool di controllo per fermare il ciclo per scartare

	CartaCfu *headCarte      = NULL, //puntatore ausiliario per scorrere la lista
			 *discardedCarta = NULL, //puntatore dove salvare la carta scartata
			 *prev           = NULL;

	do { //ciclo per ripetere lo scartare la carta
		headCarte = pPlayer->manoCarteCfu; //assegnazione lista delle carte ad headCarte
		if (headCarte == NULL) { //se non ci sono carte
			printf("Non hai carte da scartare!\n");
			stop = true; // blocco del ciclo
		} else {
			while (headCarte != NULL) { //controlla tutte le carte
				printf("[%d] ", i + 1);
				printSingleCartaCfu(headCarte);
				headCarte = headCarte->next; //passaggio alla prossima carta
				i++;
			}
			printf("[0] Non scartare nulla\n");

			do{
				printf(">>>");
				scanf("%d", &choice);
				if (choice < 0 || choice > i) {
					printf("Input non valido, riprovare");
				}
			} while (choice < 0 || choice > i);

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
				headCarte = pPlayer->manoCarteCfu; // assegnazione lista carte ad headCarte
				//ciclo per arrivare alla carta precedente di quella scelta
				for (int j = 0; j < choice; ++j) {
					prev = headCarte;
					headCarte = headCarte->next; //passaggio alla carta successiva
				}
				discardedCarta = headCarte; //la carta scartata è la successiva ad headCarte
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

void effettoDOPPIOE(bool *checkDOPPIOE) {
	*checkDOPPIOE = true;
	printf("\n ATTENZIONE!\n"
		   "Da ora in poi gli effetti AUMENTA e DIMINUISCI modificheranno il punteggio di 4 punti\n");
}

// ====================================================================================================================

// TODO !!! CONTROLLA QUESTE SUBROUTINE!!!

void effettoSBIRCIA(CartaCfu **mazzoCfu, Player *pPlayer, CartaCfu **mazzoScarti) {
	CartaCfu *carteSbirciate = NULL, //lista delle carte guardate
			 *headCfu        = NULL, //puntatore per scorrere la lista
			 *discardedCarta = NULL; //carta che viene scartata

	bool flag = false; //bool di controllo
	//variabili di conteggio delle carte nel mazzo e scelta della carta
	int nCarte,
		carta;

	// TODO ATTENZIONE DEVO CAMBIARE LOGICA MESCOLAMNETNO
	nCarte = contaCarteCfu(*mazzoCfu); //conteggio numero di carte nel mazzo
	if (nCarte < CARTE_SBIRCIABILI) { //se le carte del mazzo sono meno di quelle necessarie
		*mazzoCfu = mescolaMazzo(mazzoScarti); //rimescoliamo la pila degli scarti con il mazzo
	}
	carteSbirciate = (*mazzoCfu);
	(*mazzoCfu) = (*mazzoCfu)->next->next; //la nuova testa è la terza carta del mazzo
	carteSbirciate->next->next = NULL; //separo le due carte sbirciate dal mazzo
	headCfu = carteSbirciate;
	printf("%s sbirci le prime due carte del mazzo e ne tieni una...\n", pPlayer->username);

	// end TODO

	//ciclo for stampare le carte disponibili
	for (int i = 0; i < CARTE_SBIRCIABILI; ++i) {
		printSingleCartaCfu(headCfu);
		printf("\n");
		headCfu = headCfu->next; //passaggio alla carta successiva
	}

	do { //ciclo per permettere la scelta e ripeterla in caso di errore
		printf("Scegli la carta:");
		scanf("%d", &carta); //acquisizione scelta
		flag = false; //a ogni ciclo flag torna a false
		switch (carta) { //switch case che permette la scelta di una delle 2 carte
			case 0:
				discardedCarta = carteSbirciate->next; //la carta scartata è la seconda
				printf("Hai scelto %s!\n", carteSbirciate->name);
				break;
			case 1:
				discardedCarta = carteSbirciate; //la scartata è la prima della lista
				carteSbirciate = carteSbirciate->next; //la testa delle carte sbirciate diventa la successiva
				printf("Hai scelto %s!\n", carteSbirciate->name);
				break;
			default: //se vengono messi altri input oltre a quelli disponibili
				printf("Errore, riprovare\n");
				flag = true; //la variabile di controllo è true e si ripete il ciclo
		}
	} while (flag); //se flag è true l'immissione si ripete

	//inserimento in testa della carta scelta nella mano
	carteSbirciate->next = pPlayer->manoCarteCfu;
	pPlayer->manoCarteCfu = carteSbirciate;

	//inserimento in testa della carta scartata nella pila
	discardedCarta->next = (*mazzoScarti);
	(*mazzoScarti) = discardedCarta;
}

// ====================================================================================================================

void effettoSCAMBIAC(CartaCfu **carteGiocate, Player *playerList, bool arrRisolte[]) {
	CartaCfu *headCarte = NULL, //puntatore ausiliario per scorrere la lista
			 *pCarta1 = NULL, //puntatore per salvare la carta del giocatore 1
	         *pCarta2 = NULL, //puntatore per salvare la carta del giocatore 2
	         cartaCfu1,
	         cartaCfu2; //strutture cartaCFU ausiliarie per salvare i valori delle carte scambiate

	Player *headPlayers = playerList,
		   *pPlayer1    = NULL,
		   *pPlayer2    = NULL; //puntatori per salvare i giocatori scelti

	bool check = false; // bool di controllo

	int i        = 0, // i: indice dei cicli
		iPlayer1 = 0, // iPlayer2: indice della carta del giocatore 2
		iPlayer2 = 0; // iPlayer1: indice della carta del giocatore 1

	headCarte = (*carteGiocate); //assegnazione della lista delle carte ad headCfu

	do { //ciclo per stampare le scelte disponibili
		printf("[%d] Carta di %s:\n", i + 1, headPlayers->username);
		printSingleCartaCfu(headCarte);
		headCarte   = headCarte->next; //passaggio carta successiva
		headPlayers = headPlayers->nextPlayer; //passaggio al giocatore successivo
		i++; //incremento l'indice
	} while (headCarte != NULL); //il ciclo continua fino all'ultima carta giocata

	printf("[0] Non effettuare lo scambio\n");
	do { //ciclo per immissione scelta e possibile ripetizione in caso di input errato
		printf("Scelta:");
		scanf("%d", &iPlayer1); //acquisizione scelta
		if (iPlayer1 < 0 || iPlayer1 > i){ //errore se la scelta non rispetta quelle disponibili
			printf("Errore, riprovare\n");
		}
	} while (iPlayer1 < 0 || iPlayer1 > i); //il ciclo si ripete se la scelta non rispetta quelle disponibili

	if (iPlayer1 == 0) { //se si sceglie di non effettuare lo scambio si esce dalla subroutine
		printf("Nessuno scambio\n");
	} else {
		i=0; //reset dell'indice del ciclo
		iPlayer1--; //decremento l'indice scelto (l'opzione di scelta era incrementata di uno)
		headCarte = (*carteGiocate); //assegnazione della lista alla variabile headCfu
		headPlayers = playerList; // pPlayer parte dalla testa della lista dei giocatori
		do { //ciclo per individuare il giocatore scelto
			if (i == iPlayer1) { //quando i raggiunge l'indice del giocatore scelto
				pCarta1 = headCarte; //assegno la carta del giocatore
			} else { //altrimenti passa al giocatore successivo
				headPlayers = headPlayers->nextPlayer;
				headCarte = headCarte->next; //prossima carta
				i++; //incremento l'indice del ciclo
			}
		} while (pCarta1 == NULL); //il ciclo termina quando viene assegnata la carta

		headCarte = (*carteGiocate); //assegnazione della lista ad headCfu
		i       = 0; //reset dell'indice

		do { //ciclo per stampare i giocatori da scegliere
			if (i == iPlayer1){ //se l'indice corrisponde al giocatore scelto in precedenza
				printf("Giocatore: %s (Giocatore già scelto)\n[%d] Carta: %s (%d CFU)\n\n",
				       headPlayers->username, i, headCarte->name, headCarte->cfu);
			} else { //stampa delle altre scelte
				printf("Giocatore: %s\n[%d] Carta: %s (%d CFU)\n\n",
				       headPlayers->username, i, headCarte->name, headCarte->cfu);
			}
			headCarte   = headCarte->next; //passaggio alla prossima carta
			headPlayers = headPlayers->nextPlayer; //passaggio al giocatore successivo
			i++; //incremento dell''indice
		} while (headCarte != NULL); //il ciclo continua fino alla fine della lista delle carte giocate

		do { //ciclo per mettere la scelta e ripeterla in caso di input errato
			printf("Scegli la carta:");
			scanf("%d", &iPlayer2); //acquisizione scelta
			//errore se l'input è maggiore o minore di quelli disponibili o se coincide con il giocatore precedente
			if (iPlayer2 < 0 || iPlayer2 > i - 1 || iPlayer2 == iPlayer1) {
				printf("Errore, riprovare\n");
			}
		} while (iPlayer2 < 0 || iPlayer2 > i - 1 || iPlayer2 == iPlayer1); //in caso di errore si ripete il ciclo

		i        = 0; //reset indice
		headCarte  = (*carteGiocate); //assegnazione carte giocate ad headCfu
		pPlayer2 = playerList; //pPlayer2 parte dalla testa della lista dei giocatori

		do { //ciclo per determinare la scelta del secondo giocatore
			if (i == iPlayer2) { //quando i raggiunge la l'indice del secondo giocatore
				pCarta2 = headCarte; //assegno la carta del giocatore scelto
			} else { //altrimenti controlla il giocatore successivo
				headCarte  = headCarte->next; //carta successiva
				pPlayer2 = pPlayer2->nextPlayer; //giocatore successivo
				i++; //indice successivo
			}
		} while (pCarta2 == NULL); //il ciclo termina quando la carta viene assegnata

		/* non modifico l'ordine della lista attraverso i puntatori, modifico i contenuti e riassegno la carta
		 * successiva corretta per avere l'ordine della lista */
		cartaCfu2 = (*pCarta2); //salvo il contenuto della carta giocatore 2 nella struttura ausiliaria
		*pCarta2 = (*pCarta1); //assegno il contenuto della carta del giocatore 1 al giocatore 2
		//ristabilisco l'ordine della lista usando la carta ausiliaria
		pCarta2->next = cartaCfu2.next;
		cartaCfu1 = (*pCarta1); //salvo il contenuto della carta giocatore 1 nella struttura ausiliaria
		(*pCarta1) = cartaCfu2; //assegno il contenuto della carta del giocatore 2 al giocatore 1
		//ristabilisco l'ordine della lista usando la carta ausiliaria
		pCarta1->next = cartaCfu1.next;
		printf("%s e %s si sono scambiati le carte!\n"
		       "%s -> Carta: %s\n%s -> Carta: %s",
		       pPlayer1->username, pPlayer2->username,
		       pPlayer1->username, pCarta1->name,
			   pPlayer2->username, pCarta2->name);
		/*arrRisolte[] tiene conto delle carte che hanno risolto l'effetto con corrispondenza tra indice dell'array e
		 * posizione della carta nella lista, questo vuol dire che se due carte vengono scambiate devo scambiare anche
		 * il controllo delle risoluzioni delle carte */
		check = arrRisolte[iPlayer1];
		arrRisolte[iPlayer1] = arrRisolte[iPlayer2];
		arrRisolte[iPlayer2] = check;
	}
}

// TODO ¡¡¡CONTROLLA QUESTE SUBROUTINE¡¡¡