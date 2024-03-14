#include "turno.h"
#include "carteOstacolo.h"
#include "giocatori.h"
#include "memoria.h"
#include "mazzoCfu.h"
#include "saves.h"

// ============ TURNO - GIOCA CARTA ====================================================================================
/**
 * giocaCarta() gestisce l'acquisizione da parte di un giocatore della carta che vuole giocare e aggiungere la carta
 * alla lista delle carte giocate
 * @param manoCarteCfu CartaCfu **: doppio puntatore alla mano di carte del player
 * @param mazzoScarti CartaCfu **: doppio puntatore al mazzo di scarto
 * @param mazzoCarteCfu CartaCfu **: doppio puntatore al mazzo di pesca
 * @param rimescolaMano bool: è una flag che indica se è concesso il rimescolamento della mano in caso non si abbiano
 * carte punto
 * @return CartaCfu *: la carta scelta
 */
CartaCfu *chooseCarta(CartaCfu **manoCarteCfu,
					  CartaCfu **mazzoCarteCfu, CartaCfu **mazzoScarti,
					  bool rimescolaMano) {
	CartaCfu *currMano    = NULL,      // Puntatore della lista delle carte in mano
	         *choosenCard = NULL,               // Pointer alla carta giocata in questo turno
	         *prev        = NULL;               // Pointer alla carta precedente quella giocata
	int      choice       = 0;                  // Indice della carta scelta dal player
	bool instant,                               // Flag carta istantanea per chiedere una carta diversa
	     tutteIstantanee  = true,               // Flag mano tutta di carte istantanee per rimescolare la mano
		 giocabile        = true;               // Flag possibilità di scegliere carta


	do {
		currMano = *manoCarteCfu;
		// Controllo che il giocatore possa giocare carte
		tutteIstantanee = tutteIstantaneeCheck(currMano);

		// Se ha almeno una carta giocabile posso stampare la sua mano
		if (tutteIstantanee == false) {
			printMano(currMano);
		}

		// Se tutte le carte in mano sono istantanee e il rimescolamento della mano è concesso
		if (tutteIstantanee == true && rimescolaMano == true){
			printf("\n Tutte le carte che hai in mano sono istantanee, scarta tutta la tua mano e pesca 5 nuove carte");
			enterClear();
			// Posso scartare le carte e prendere una nuova mano
			*mazzoScarti = scartaCarte(manoCarteCfu, *mazzoScarti);
			*manoCarteCfu = distribuisciCarte(*manoCarteCfu, mazzoCarteCfu, mazzoScarti);

		// Se si hanno solo carte istantanee in mano e non è concesso il rimescolamento, giocabile è false
		} else if (tutteIstantanee == true && rimescolaMano == false) {
			printf("Non puoi giocare nessuna carta in questo momento");
			giocabile = false;
		}
		// ripeto il ciclo fino a quando il giocatore ha solo carte istantanee o non può rimescolare la mano
	} while (tutteIstantanee == true && rimescolaMano == true);

	// Se il giocatore può giocare delle carte
	if (giocabile) {
		do {
			// Acquisisco quale carta vuole giocare
			choice = acquisisciInputInt(0, CARTE_PER_MANO - 1);
			// Trovo la carta nella mano
			choosenCard = findCartaCfu(*manoCarteCfu, choice);

			// Controllo se è istantanea, se non è istantanea posso restituirla, altrimenti chiedo nuovamente al
			// giocatore
			instant = isIstantanea(choosenCard);
			if (instant) {
				printf("\nLa carta scelta è una carta istantanea, scegline un'altra\n");
			}
		} while (instant);
	} else {
		// Se il giocatore non può giocare carte e il giocatore non può giocare carte restituisco NUKK
		choosenCard = NULL;
	}

	return choosenCard;
}

/**
 * giocaCarte() è la subroutine che si occupa di gestire l'azione giocaCarta del giocatore
 * @param turno Turno *: puntatore alla subroutine del turni
 * @param pPlayer Player *: puntatore al player corrente
 * @param mazzoScarti CartaCfu **: doppio puntatore al mazzo degli scarti
 * @param mazzoCfu CartaCfu **: doppio puntatore al mazzo di pesca
 * @param fLog FILE *: puntatore al file di Log
 * @param spareggioFlag bool: flag di controllo per considerazione caratteristiche personaggi
 */
void giocaCarta(Turno *turno, Player *pPlayer,
		   CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
		   FILE *fLog, bool spareggioFlag) {
	CartaCfu *choosenCard = NULL,
			 **manoCarteCfu = NULL;

	manoCarteCfu = &(pPlayer->manoCarteCfu);

	// con chooseCarta() ottengo la carta che il giocatore intende giocare
	choosenCard = chooseCarta(manoCarteCfu, mazzoCfu, mazzoScarti, spareggioFlag);

	// Se la carta scelta è valida
	if (choosenCard != NULL) {
		// Posso estrarla e aggiungerla alla lista di carte gioacrte
		choosenCard = estraiCartaCfu(manoCarteCfu, choosenCard);
		// Stampo su log
		logPrintLn(fLog, turno->numTurno, pPlayer->username, choosenCard->name);

		// Aggiungo la carta IN CODA alla lista di carte giocate in questo turno
		cartaCfuInCoda(&(turno->carteGiocate), choosenCard);
	} else {
		exit(ERR_PLAY_CARD);
	}
}

// ============ TURNO - INFO GIOCATORE =================================================================================
/**
 * infoGiocateori() è la subroutine che data la lista dei giocatori si occupa di stampare le informazioni di uno
 * @param listaGiocatori Player *: lista di giocatori
 * @param currentPlayer Player *: giocatore attuale
 * @param nPlayers int: numero di giocatori in corso
 */
void infoGiocatori(Player *listaGiocatori,
				   Player *currentPlayer,
				   int nPlayers) {
	Player *curr          = listaGiocatori,
		   *choosenPlayer = listaGiocatori;

	int count = 0,
		input;

	curr          = listaGiocatori;
	choosenPlayer = listaGiocatori;


	printf("\nDi quale giocatore vuoi avere informazioni?\n");

	// Stampa dei giocatori dei quali si possono avere informazioni
	for (int i = 0; i < nPlayers && curr != NULL; ++i) {
		// Se il giocatore è quello attuale non stampo il corrispondente indice
		if (strcmp(curr->username, currentPlayer->username) != 0) {
			printf("[%d] %s\n", count, curr->username);
		}
		curr = curr->nextPlayer;
		count++;
	}

	// Acquisizione del giocatore del quale si vogliono avere infomrmazioni
	input = acquisisciInputInt(0, count);

	// Scorro fino al giocatore che devo stampare
	for (int i = 0; i < input; ++i) {
		if (choosenPlayer->nextPlayer != NULL){
			choosenPlayer = choosenPlayer->nextPlayer;
		}
	}
	// Stampa giocatore richiesto
	printGiocatore(choosenPlayer);
}

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
/**
 * calcolaPunteggio() calcola il punteggio ottenuto da ciascun giocatore in un turno, considerando i punti cfu di
 * ciascuna carta e i bonus/malus di ogni personaggio e li salva in turno.points come array allocato dinamicamente
 * @param turno Turno *: turno di gioco
 * @param playerList Player *: lista di giocatori
 * @param nPlayers int: numero di giocatori in gioco
 * @param characters bool: indica se le personalità dei personaggi debbano inficiare sul calcolo dei punteggi
 */
void calcolaPunteggio(Turno *turno, Player *playerList,
					  int nPlayers, bool characters) {
	Player *currPlayer = NULL;
	CartaCfu *currCfu  = NULL;
	int modifier       = 0;

	currPlayer = playerList;
	currCfu    = turno->carteGiocate;

	// Alloco array di punti
	if (turno->points == NULL) {
		turno->points = (int *)calloc(nPlayers, sizeof(int));
		if (turno->points == NULL){
			exit(ERR_FAIL_ALLOC_POINTS);
		}
	}
	// Ciclo sui giocatori
	for (int i = 0; i < nPlayers; ++i) {
		// Se characters è vero devo considerare anche i bonus/malus dei giocatori
		modifier = characters ? currPlayer->character.bonusMalus[turno->cartaOstacolo->type - 1] : 0;

		// Salvo il punteggio dell'i-esimo giocatore in points[i]
		turno->points[i] = currCfu->cfu + modifier;
		currPlayer = currPlayer->nextPlayer;
		currCfu    = currCfu->next;
	}
}

/**
 * printPunti() stampa i punteggi dei giocatori in un turno
 * @param turno Turno *: struttura turno
 * @param playerList Player *: lista di giocatori
 * @param nPlayers int: numero di giocatroi
 */
void printPunti(Turno *turno, Player *playerList, int nPlayers) {
	Player *pPlayer = NULL; // puntatore al player attuale
	pPlayer = playerList; // puntatore al player attuale

	printf("\nPunti Cfu del turno %d\n", turno->numTurno);
	// Ciclo sul numero di giocatori
	for (int i = 0; i < nPlayers; ++i) {
		// Per ogni giocatore stampo usernamen e punteggio corrispondente
		printf("%31s: %d\n", pPlayer->username, turno->points[i]);
		pPlayer = pPlayer->nextPlayer;
	}
	printf("+------------------------------+\n");
}

/**
 * winnersLosers() occupa di identificare i vincitori e i perdenti di ogni turno, analizzando
 * i punti totalizzati nel punto e crea una lista di giocatori perdenti e vincenti
 * @param turno Turno *: struttura che identifica ogni turno
 * @param playersList Player *: lista di giocatori partecipanti
 * @param nPlayers int: numero di giocatori attualmente in partita
 */
void winnersLosers(Turno *turno, Player *playersList, int nPlayers) {
	Player *curr = playersList;

	// Se i cfu di vittoria sono uguali ai cfu per perdere, i giocatori sono tutti in pareggio e non ci sono
	// vincitori o perdentei, altrimenti
	if (turno->cfuToWin != turno->cfuToLose) {
		// Ciclo sul numero di giocatori
		for (int i = 0; i < nPlayers; ++i) {
			// Se l'i-esimo giocatore
			// Ha i punteggi come quelli per vincere
			if (turno->points[i] == turno->cfuToWin) {
				// Ne creo una copia sulla lista winners
				turno->winners = addCopyOfPlayerInCoda(turno->winners, curr);
			}
			// Ha i punteggi come quelli per perdere
			if (turno->points[i] == turno->cfuToLose) {
				// Ne creo una copia sulla lista losere
				turno->losers = addCopyOfPlayerInCoda(turno->losers, curr);
			}
			curr = curr->nextPlayer;
		}
	}
}

// ============ SPAREGGI ===============================================================================================
/**
 * gestisciSpareggi() si occupa di chiamare eventuali spareggi tra i giocatori
 * @param countLosers int: numero di giocatori perdenti
 * @param turno Turno *: struttura che identifica il turno principaòe
 * @param mazzoCfu CartaCfu **: mazzo di pesac delle carte
 * @param mazzoScarti CartaCfu **: mazzo scarti carte cfu
 * @param fLog FILE *: puntatore al file di log
 * @return Player *: giocatore perdente finale
 */
Player *gestisciSpareggi(int countLosers, Turno *turno, CartaCfu **mazzoScarti, CartaCfu **mazzoCfu, FILE *fLog) {
	Turno spareggio = {}; // Struttura turno che identifica questo spareggio
	Player *currPlayer  = NULL,
		   *pLoser      = NULL;
	CartaCfu *currCarta = NULL;
	bool loser = false;

	currPlayer = turno->losers;

	printf("\nSpareggio tra:\n");
	while (currPlayer != NULL) {
		printf("%s\n", currPlayer->username);
		currPlayer = currPlayer->nextPlayer;
	}

	currPlayer = turno->losers;
	// Ciclo sui giocatori, per permettere di giocare le carte
	while (!loser && currPlayer != NULL) {
		// Controllo che il giocatore abbia carte da giocare questo turno
		currCarta = currPlayer->manoCarteCfu;
		if (contaCarteCfu(currCarta) > 0 && !tutteIstantaneeCheck(currCarta)) {
			printf("\n%s:\n", currPlayer->username);
			giocaCarta(&spareggio, currPlayer, mazzoCfu, NULL, fLog, SPAREGGIO);
			currPlayer = currPlayer->nextPlayer;
		} else {
			// Altrimenti lo aggiungo alla lista dei perdenti
			addCopyOfPlayerInCoda(spareggio.losers, currPlayer);
			// E lascio il loop
			loser = true;
		}
	}

	currPlayer = turno->losers;
	// Se non c'è un perdente per mancanzza di carte in mano
	if (!loser) {
		// Calcolo punteggi
		calcolaPunteggio(&spareggio, currPlayer, countLosers, SPAREGGIO);
		printPunti(&spareggio, currPlayer, countLosers);

		// Vincitori e perdenti
		winnersLosers(&spareggio, currPlayer, countLosers);
		printLosers(spareggio.losers);
	}

	countLosers = contaLosers(&spareggio); // Conta i giocatori che hanno perso

	// Se il giocatore perdente è solo uno, posso restituirlo come perdente
	if (countLosers == 1 || loser) {
		currPlayer = turno->losers;
		while (spareggio.losers != NULL) {
			if (strcmp(spareggio.losers->username, currPlayer->username) == 0) {
				pLoser = currPlayer;
			}
			spareggio.losers = spareggio.losers->nextPlayer;
		}
	} else {
		// Altrimenti passo a un nuovo spareggio
		printf("\nRisoluzione spareggi");
		pLoser = gestisciSpareggi(countLosers, &spareggio, mazzoScarti, mazzoCfu, fLog);
	}

	return pLoser;
}

// ============ VINCITORI ==============================================================================================
/**
 * printWinners() data una lista di giocatori, ne stampa i nomi, formattati come lista di vincitori
 * @param playerList Player *: lista di giocatori da stampare
 */
void printWinners(Player *playerList) {
	Player *curr = playerList;
	printf("\nVincitori:\n");
	while (curr != NULL){
		printf("%s\n", curr->username);
		curr = curr->nextPlayer;
	}
}

/**
 * assegnaPunti() assegna ai giocatori vincenti i punti cfu che gli spettano
 * @param turno Turno *: struttura turno, contiene i punti per vincere e i punti di ciascun giocatore
 * @param playerList Player *: lista di giocatori in partita
 * @param nPlayers int: numero di giocatori in partita
 */
void assegnaPunti(Turno *turno, Player *playerList, int nPlayers){
	Player *curr      = playerList;
	int pointsWinners = turno->cfuToWin; // Salvo in pointsWinners i punti che devono aver fatto i giocatori per
	                                     // vincere per non dover fare l'accesso alla memoria di turno ripetutamente

	for (int i = 0; i < nPlayers; ++i) {         // Per ogni giocatore
		if (pointsWinners == turno->points[i]) { // Se i punti fatti in questo turno sono uguali ai punti per vincere
			curr->cfu += pointsWinners;          // Allora incremento i suoi punti cfu
		}
		curr = curr->nextPlayer;
	}
}

// ============ PERDENTI ===============================================================================================
/**
 * printLosers() data una lista di giocatori, ne stampa i nomi, formattati come lista di perdenti
 * @param playerList Player *: lista di giocatori da stampare
 */
void printLosers(Player *playerList) {
	Player *curr = playerList;
	printf("\nPerdenti:\n");
	while (curr != NULL){
		printf("%s\n", curr->username);
		curr = curr->nextPlayer;
	}
}

/**
 * contaLosers() conta quanti giocatori sono perdenti
 * @param turno Turno *: struttura Turno
 * @return int: numero di giocatori perdenti
 */
int contaLosers(Turno *turno) {
	Player *curr    = turno->losers;
	int losersCount = 0;

	// Controlla quanti perdenti ci sono nel turno
	while (curr != NULL) {
		losersCount++;
		curr = curr->nextPlayer;
	}
	return losersCount;
}

/**
 * puntiCarteOstacolo() a termine turno assegna ai giocatori tanti punti quante carte ostacolo hanno
 * @param playerList Player *: lista di giocatori
 */
void puntiCarteOstacolo (Player *playerList) {
	Player        *currPlayer   = playerList; // Testa della lista giocatori
	CartaOstacolo *currOstacoli = NULL;       // Testa della lista di ostacoli di ogni giocatore
	int countOstacoli           = 0;          // Contatore di ostacoli per ogni giocatore

	while (currPlayer != NULL) {    // Per ogni giocatore
		countOstacoli = contaOstacoli(currPlayer->listaCarteOstacolo); // Conto le carte ostacolo
		currPlayer->cfu += countOstacoli;    // Incrementa i CFU del giocatore di quante ostacolo ha
		currPlayer = currPlayer->nextPlayer; // Passa al giocatore successivo
	}
}

/**
 * ostacoloAlLoser() si occupa di trovare il perdente nella lista dei giocatori e assegnarli la carta ostacolo che
 * gli spetta
 * @param turno Turno *: struttura turno
 * @param playersList Player *: lista dei giocatori
 * @param pLoser Player *: copia del giocatore perdente
 */
void ostacoloALoser(Turno *turno, Player *playersList, Player *pLoser) {
	CartaOstacolo *ostacolo = NULL;
	ostacolo = turno->cartaOstacolo;

	while (playersList != NULL) {
		if (strcmp(playersList->username, pLoser->username) == 0) {
			playersList->listaCarteOstacolo = ostacoloInTesta(playersList->listaCarteOstacolo, ostacolo);
		}
		playersList = playersList->nextPlayer;
	}

}
// ============ CHIUSURA ===============================================================================================
/**
 * end() è la subroutine per chiudere una partita che si occupa di liberare la memoria
 * @param playerList Player *: lista giocatori
 * @param mazzoCfu CartaCfu *: mazzo di pesca Cfu
 * @param mazzoScarti CartaCfu *: mazzo di scarto Cfu
 * @param mazzoOstacoli CartaOstacolo *: mazzo carte ostacolo
 */
void end(Player *playerList, CartaCfu *mazzoCfu, CartaCfu *mazzoScarti, CartaOstacolo *mazzoOstacoli, Turno *turno) {
	// Chiamo la funzione per liberare la memoria per ogni lista aloocata
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	freeTurno(turno);
	exit(EXIT_SUCCESS);
}

/**
 * freeTurno() libera tutti gli oggetti allocati sull'heap in una struttura turno
 * @param turno Turno *: struttura turno da liberare
 */
void freeTurno(Turno *turno) {
	turno->points = freeIntArr(turno->points);
	turno->cartaOstacolo = freeOstacoli(turno->cartaOstacolo);
	turno->carteGiocate = freeCfu(turno->carteGiocate);
	turno->winners = freeGiocatore(turno->winners);
	turno->losers = freeGiocatore(turno->losers);
}
