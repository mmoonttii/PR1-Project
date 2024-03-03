#include "turno.h"
#include "carteOstacolo.h"
#include "giocatori.h"
#include "memoria.h"
#include "mazzoCfu.h"
#include "salvataggiLog.h"

// ============ TURNO - FASE 1 =========================================================================================

/**
 * Acquisisci azione che si occupa di ricevere l'input del giocatore che corrisponde a un azione da compiere
 * all'inzio del turno
 * @return
 */
int acquisisciAzione() {
	int input;
	printf("\nAzioni:\n"
	       "\t[1] Gioca una carta Cfu\n"
	       "\t[2] Controlla lo stato di altri giocatori (Cfu e carte ostacolo\n"
	       "\t[0] Esci dalla partita\n"
	       ">>> ");
	do {
		scanf("%d", &input);

		if (input > 2 || input < 0) {
			printf("Input non valido, riprovare\n"
			       ">>> ");
		}
	} while (input > 2 || input < 0);
	return input;
}

/**
 * giocaCartaTurno() è la subroutine che si occupa di gestire l'acquisizione da parte di un giocatore della carta che
 * vuole giocare e aggiungere la carta alla lista delle carte giocate
 * @param manoCarteCfu é la lista delle carte in mano al giocatore
 */
CartaCfu *chooseCarta(CartaCfu **manoCarteCfu, CartaCfu **mazzoScarti, CartaCfu *mazzoCarteCfu, bool rimescolaMano) {
	CartaCfu *headMano    = *manoCarteCfu,      // Puntatore della lista delle carte in mano
	         *choosenCard = NULL,               // Pointer alla carta giocata in questo turno
	         *prev        = NULL;               // Pointer alla carta precedente quella giocata
	int      choice       = 0;                  // Indice della carta scelta dal player
	bool instant,                               // Flag carta istantanea per chiedere una carta diversa
	     tutteIstantanee  = true,               // Flag mano tutta di carte istantanee per rimescolare la mano
		 giocabile        = true;

	do {
		tutteIstantanee = tutteIstantaneeCheck(*manoCarteCfu);
		if (!tutteIstantanee) {
			printMano(*manoCarteCfu);
		}
		if (tutteIstantanee == true && rimescolaMano == true){
			printf("\n Tutte le carte che hai in mano sono istantanee, scarta tutta la tua mano e pesca 5 nuove carte");
			enterClear();
			scartaCarte(manoCarteCfu, mazzoScarti);
			*manoCarteCfu = distribuisciCarte(*manoCarteCfu, &mazzoCarteCfu, mazzoScarti);
		} else if (tutteIstantanee == true && rimescolaMano == false) {
			printf("Non puoi giocare nessuna carta in questo momento");
			giocabile = false;
		}
	} while (tutteIstantanee == true && rimescolaMano == true);

	if (giocabile) {
		do {
			choice = acquisisciCarta(CARTE_PER_MANO);
			choosenCard = findCartaCfu(manoCarteCfu, choice);

			// Check istantanea
			instant = isIstantanea(choosenCard) ? true : false;
			if (instant) {
				printf("\nLa carta scelta è una carta istantanea, scegline un'altra carta\n");
			}
		} while (instant);
	} else {
		choosenCard = NULL;
	}
	return choosenCard;
}

void giocaCartaTurno(Turno *turno, Player *pPlayer, CartaCfu **mazzoScarti, CartaCfu *mazzoCfu, FILE *fLog, bool
spareggioFlag) {
	CartaCfu *choosenCard = NULL,
			 **manoCarteCfu = NULL;

	manoCarteCfu = &(pPlayer->manoCarteCfu);

	choosenCard = chooseCarta(manoCarteCfu, mazzoScarti, mazzoCfu, spareggioFlag);
	if (choosenCard != NULL) {
		choosenCard = estraiCartaCfu(manoCarteCfu, choosenCard);
		logPrintLn(fLog, turno->numTurno, pPlayer->username, choosenCard->name);
		cartaCfuInCoda(&(turno->carteGiocate), choosenCard);
	}
}

int acquisisciCarta(int i) {
	int choice;

	do {
		printf("[0 - %d]>>> ", i - 1);
		scanf("%d", &choice);

		if (choice < 0 || choice > i) {
			printf("\n\t%d non è una scelta valida, riprova", choice);
		}
	} while (choice < 0 || choice > i);

	return choice;
}

/**
 * infoGiocateori() è la subroutine che data la lista dei giocatori si occupa di stampare le informazioni di uno
 * @param listaGiocatori è la lista dei giocatori
 */
void infoGiocatori(Player *listaGiocatori, Player *currentPlayer, int nPlayers) {
	Player *head = listaGiocatori,
		   *choosenPlayer = listaGiocatori;
	int count = 0,
		input;

	printf("\nDi quale giocatore vuoi avere informazioni?\n");

	// Stampa dei giocatori dei quali si possono avere informazioni
	for (int i = 0; i < nPlayers; ++i) {
		if (strcmp(head->username, currentPlayer->username) != 0) {
			printf("[%d] %s\n", count, head->username);
		}
		if (head->nextPlayer != NULL) {
			head = head->nextPlayer;
		}
		count++;
	}
	// Acquisizione del giocatore del quale si vogliono avere infomrmazioni
	do {
		printf("\n>>> ");
		scanf("%d", &input);

		if (input < 0 || input > count){
			printf("Valore inserito non valido, riprovare.\n");
		}
	} while (input < 0 || input > count);

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
 * calcolaPunteggio() è la subroutine che si occupa di calcolare il punteggio ottenuto da ciascun giocatore in un
 * turno, considerando i punti cfu di ciascuna carta e i bonus/malus di ogni personaggio e li salva in un array
 * allocato dinamicamente nella struttura turno
 * @param turno è la struttura di tipo Turno che rappresenta un turno di gioco
 * @param playerList è la lista dei giocatori nella partita
 * @param nPlayers è il numero di giocatori ing iococ
 * @param characters indica se le personalità dei personaggi debbano inficiare sul calcolo dei punteggi
 */
void calcolaPunteggio(Turno *turno, Player *playerList, int nPlayers, bool characters) {
	Player *headPlayer = playerList;
	CartaCfu *headCarte = turno->carteGiocate;
	int modifier = 0;

	if (turno->points == NULL) {
		turno->points = (int *)calloc(nPlayers, sizeof(int));
		if (turno->points == NULL){
			exit(ERR_FAIL_ALLOC_POINTS);
		}
	}
	for (int i = 0; i < nPlayers; ++i) {
		// Se characters è vero devo considerare anche i bonus/malus dei giocatori
		modifier = characters ? headPlayer->character.bonusMalus[turno->cartaOstacolo->type - 1] : 0;

		turno->points[i] = headCarte->cfu + modifier;
		headPlayer = headPlayer->nextPlayer;
		headCarte = headCarte->next;
	}
}

void printPuntiParziali(Turno *turno, Player *playerList, int nPlayers) {
	Player *pPlayer = playerList;

	printf("\nPunti Cfu del turno %d\n", turno->numTurno);
	for (int i = 0; i < nPlayers; ++i) {
		printf("%31s: %d\n", pPlayer->username, turno->points[i]);
		pPlayer = pPlayer->nextPlayer;
	}
	printf("+------------------------------+\n");
}

/**
 * winnersLosers() è la subroutine che si occupa di identificare i vincitori e i perdenti di ogni turno, analizzando
 * i punti totalizzati nel punto e crea una lista di giocatori perdenti e vincenti
 * @param turno è la struttura che identifica ogni turno
 * @param playersList è la lista di giocatori partecipanti
 * @param nPlayers è il numero di giocatori attualmente in partita
 */
void winnersLosers(Turno *turno, Player *playersList, int nPlayers) {
	Player *pPlayer = playersList;
	// minMax calcola il punteggio massimo ottenuto dai giocatori, e quindi il punteggio per vincere e il punteggio
	// minimo ottenuto, quindi il punteggio per perdere
	minMax(turno->points, nPlayers, &turno->cfuToLose, &turno->cfuToWin);

	if (turno->cfuToWin != turno->cfuToLose) {
		for (int i = 0; i < nPlayers; ++i) {
			if (turno->points[i] == turno->cfuToWin) {
				turno->winners = addPlayerInCoda(turno->winners, pPlayer);
			}
			if (turno->points[i] == turno->cfuToLose) {
				turno->losers = addPlayerInCoda(turno->losers, pPlayer);
			}
			pPlayer = pPlayer->nextPlayer;
		}
	} else {

	}
}

/**
 * printWinners() è la subroutine che data una lista di giocatori, ne stampa i nomi, formattati come lista di vincitori
 * @param playerList è la lista di giocatori da stampare
 */
void printWinners(Player *playerList) {
	Player *head = playerList;
	printf("\nVincitori:\n");
	while (head != NULL){
		printf("%31s\n", head->username);
		head = head->nextPlayer;
	}
}
/**
 * printLosers() è la subroutine che data una lista di giocatori, ne stampa i nomi, formattati come lista di perdenti
 * @param playerList è la lista di giocatori da stampare
 */
void printLosers(Player *playerList) {
	Player *head = playerList;
	printf("\nPerdenti:\n");
	while (head != NULL){
		printf("%31s\n", head->username);
		head = head->nextPlayer;
	}
}
// ============ VINCITORI ==============================================================================================
/**
 * assegnaPunti() è la subroutine che assegna ai giocatori vincenti i punti cfu che gli spettano
 * @param turno è la struttura turno, che contiene i punti necessari a vincere e i punti parziali di ciascun giocatore
 * @param playerList è la lista di giocatori in partita
 * @param nPlayers è il numero di giocatori in partita
 */
void assegnaPunti(Turno *turno, Player *playerList, int nPlayers){
	Player *pPlayer = playerList;
	int pointsWinners = turno->cfuToWin; // Salvo in pointsWinners i punti che devono aver fatto i giocatori per
	                                     // vincere per non dover fare l'accesso alla memoria di turno ripetutamente

	for (int i = 0; i < nPlayers; ++i) {         // Per ogni giocatore
		if (pointsWinners == turno->points[i]) { // Se i punti fatti in questo turno sono uguali ai punti per vincere
			pPlayer->cfu += pointsWinners;       // Allora incremento i suoi punti cfu
		}
		pPlayer = pPlayer->nextPlayer;
	}
}
// ============ SPAREGGI ===============================================================================================

/**
 * contaLosers() è la funzione che si occupa di controllare se sia necessario far partire spareggi tra i giocatori
 * perdenti, in caso questo non sia necessario, chiama la funzione ostacoloInCoda() che da la carta ostacolo al
 * giocatore perdente
 * @param turno è la struttura di tipo Turno che identifica un turno in particolare
 * @param playerList è la lista dei giocatori in partita
 */
int contaLosers(Turno *turno, Player *playerList) {
	Player *losersHead = turno->losers,
	       *pPlayer = playerList;
	int losersCount = 0;

	// Controlla quanti perdenti ci sono nel turno
	while (losersHead != NULL) {
		losersCount++;
		losersHead = losersHead->nextPlayer;
	}

	return losersCount;
}

/**
 * puntiCarteOstacolo() è la subroutine che a termine del turno assegna ai giocatori tanti punti quante carte
 * ostacolo hanno
 * @param playerList è la lista di giocatori
 */
void puntiCarteOstacolo (Player *playerList){
	Player        *headPlayer   = playerList; // Testa della lista giocatori
	CartaOstacolo *headOstacoli = NULL;       // Testa della lista di ostacoli di ogni giocatore
	int countOstacoli = 0;                    // Contatore di ostacoli per ogni giocatore

	while (headPlayer != NULL) {    // Per ogni giocatore
		countOstacoli = 0;
		headOstacoli = headPlayer->listaCarteOstacolo;

		while (headOstacoli != NULL) { // Per ogni carta ostacolo nella sua plancia
			countOstacoli++;           // Conta carte ostacolo
			headOstacoli = headOstacoli->next;
		}

		headPlayer->cfu += countOstacoli; // Incrementa i CFU del giocatore di quante ostacolo ha
		headPlayer = headPlayer->nextPlayer; // Passa al giocatore successivo
	}
}

// ============ CHIUSURA ===============================================================================================

/**
 * end() è la subroutine per chiudere una partita che si occupa di liberare la memoria
 * @param mazzoCfu
 * @param mazzoOstacoli
 * @param playerList
 * @param mazzoScarti
 */
void end (CartaCfu *mazzoCfu, CartaOstacolo *mazzoOstacoli, Player *playerList, CartaCfu *mazzoScarti){
	mazzoCfu      = freeCfu(mazzoCfu);
	mazzoOstacoli = freeOstacoli(mazzoOstacoli);
	playerList    = freeGiocatore(playerList);
	mazzoScarti   = freeCfu(mazzoScarti);
	exit(EXIT_SUCCESS);
}
