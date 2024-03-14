#include "giocatori.h"
#include "carteOstacolo.h"
#include "memoria.h"
#include "turno.h"
#include "mazzoCfu.h"

// ============ PERSONAGGI =====================================================
/**
 * parseCharacters() legge il file dei personaggi e conserva le informazioni di ogni personaggio in un'array di
 * sturtture personaggio
 *
 * Una struttura characters nel campo bonusMalus ha un array dove vengono salvati gli eventuali bonus e malus di
 * ogni giocatore per ogni tipologia di ostacolo
 *
 * @param fPersonaggi FILE *: puntatore al file da leggere [personaggi.txt]
 * @param characters Character[]: array delle strutture personaggio
*/
void parseCharacters(FILE *fPersonaggi, Character characters[]) {
	// Per ogni personaggio
	for (int i = 0; i < N_PERSONAGGI; i++){
		// Per ogni ostacolo
		for (int j = 0; j < N_OSTACOLI; j++){
			// Leggo i bonus/malus
			fscanf(fPersonaggi, "%d ", &characters[i].bonusMalus[j]);
		}
		// LEggo il nome del personaggio
		fscanf(fPersonaggi, "%[^\n]31s", characters[i].name);
		fscanf(fPersonaggi, "\n");
	}
}

/**
 * printBonusMalus() è la subroutine che stampa i bonus e i malus di un personaggio
 * @param arr int[]: array corrispondente a ciascun personaggio
 */
void printBonusMalus(int arr[N_OSTACOLI]){
	char *types[] = {"Studio", "Sopravvivenza", "Sociale", "Esame"};

	// Per ogni tipo di ostacolo, stampa bonus/malus corrispondente
	printf("Bonus e Malus:\n");
	for (int j = 0; j < N_OSTACOLI; ++j) {
		printf("|\t%13s: %d\n", types[j], arr[j]);
	}
}

/**
 * printCharacter() è la subroutine per stampare il personaggio di un giocatrore
 * @param pCharacter Character *: puntatpre a personaffio
 */
void printCharacter(Character *pCharacter){
	printf("Personaggio: %s\n"
	       "|\t", pCharacter->name);
	// Stampo i binus/malus usnado la subtoutine printBonusMalus()
	printBonusMalus(pCharacter->bonusMalus);

}

// ============ IO ============================================================
/**
 * acquisisciNumGiocatore() acquisisce il numero dei partecipanti alla partita
 * La funzione si occupa di controllare che l'input sia valido e invita a ritentare in caso di input non valido
 * @return int: numero di giocatori alla partita
 */
int acquisisciNumGiocatori() {
	int nGiocatori;
	printf("Quanti giocatori parteciperanno oggi? [2-4]\n");

	do {
		printf(">>> ");
		scanf("%d", &nGiocatori);

		// Ripeto il ciclo fino a quando l'input non è valido
		if (nGiocatori < 2 || nGiocatori > 4){
			printf("Il numero di giocatori deve essere compreso tra 2 e 4:\n"
			       "\t controlla l'input o organizzatevi in squadre :)\n");
		}
	} while (nGiocatori < 2 || nGiocatori > 4);

	return nGiocatori;
}

/**
 * printGiocatore() stampa i dati di un giocatore
 * @param pPlayer Player *: è il puntatore a un giocatore
 */
void printGiocatore(Player *pPlayer) {
	printf("\nGiocatore:\n");
	printf("|\t Username: %s\n"
	       "|\t Cfu: %d\n"
	       "|\t ",
	       pPlayer->username, pPlayer->cfu);

	// Per stampara  il personaggio corrispondente uso la subroutine apposta
	printCharacter(&pPlayer->character);

	if (pPlayer->listaCarteOstacolo != NULL) {
		printf("\nCarte ostacolo:");
		printOstacoli(pPlayer->listaCarteOstacolo);
	}
	printf("+----------------------------------------\n");
}

/**
 * printGiocatori() è la subroutine che stampa la lista dei giocatori
 * @param listaGiocatori Player *:  lista di giocatori da stampare
 */
void printGiocatori(Player *listaGiocatori) {
	Player *curr = listaGiocatori;
	printf("\n=== GIOCATORI ===\n");

	// Fino a quando curr indica un giocatore valido
	for (int i = 0; curr != NULL; ++i) {
		// Stampa le informazioni
		printGiocatore(curr);
		// Scorri al giocatore successivo
		curr = curr->nextPlayer;
	}
}

// ============ LIST MANAGEMENT ===============================================
/**
 * addPlayerInTesta() è la subroutine che data una lista di player ne aggiunge un'altro in testa
 * @param playersList Player *: puntatore alla lista di player
 * @param newPlayer Player *: puntatore al Player da aggiungere
 * @return Plyr *: lista con la nuova testa
 */
Player *addPlayerInTesta(Player *playersList, Player *newPlayer) {
	newPlayer->nextPlayer = playersList; // Al player attuale accodo la lista di player gia presenti
	playersList = newPlayer; // Salvo la nuova testa
	return playersList;
}

/**
 * initGiocatori() è la funzione che, dati il numero dei giocatori e il mazzo di carte Cfu, restituisce una lista di
 * giocatori, occupandosi di inizializzarla, assegnando un username, personaggio e una mano di cinque carte iniziali
 * @param nGiocatori int: numero di giocatori che partecipano
 * @param mazzoCfu CartaCfu **: doppio puntatore al mazzo delle carte Cfu, passata per permettere l'assegnamento della
 * 									mano iniziale di carte
 * @param mazzoScarti CartaCfu **: doppio puntatore al mazzo degli scarti, passato per permettere il rimescolamento,
 * 									in caso finiscano le carte nel mazzo di pesca
 * @param personaggi Character[]: array delle strutture  personaggi
 * @return Player *: lista dei giocatori partecipanti alla partita
 */
Player *initGiocatori(int nGiocatori, Character personaggi[],
					  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti) {
	Player *playersList = NULL, // Testa della lista dei giocatori
	       *newPlayer   = NULL; // Nuovo personaggio in allocazione

	char temp[STR_LEN + 1] = {};
	int k; // Indice per l'accesso all'array di personaggi

	Character emptyCharacter  = {}; // Struttura personaggio ausiliaria vuota

	printf("\n=== PARTECIPANTI ===\n");
	// Per ogni giocatore
	for (int i = 0; i < nGiocatori; ++i) {
		newPlayer = allocaGiocatore();
		// Richiedi username
		printf("\nGIOCATORE %d\n", i);
		// Mi assicuro che il nome utente non sia già utilizzato
		do {
			printf("\nInserire username: ");
			scanf(" %[^\n]31s", temp);

			if (!checkUsernames(playersList, temp)) {
				strcpy(newPlayer->username, temp);
			} else {
				printf("\nUsername già inserito, riprova");
			}
		} while (checkUsernames(playersList, temp));

		// Inizializza la struttura con 0 cfu, mano di carte, carte ostacolo e prossimo gicatore a null
		newPlayer->cfu                = 0; // Punteggio di partenza
		newPlayer->manoCarteCfu       = NULL;
		newPlayer->manoCarteCfu       = distribuisciCarte(newPlayer->manoCarteCfu, mazzoCfu, mazzoScarti); // Mano iniziale
		newPlayer->listaCarteOstacolo = NULL;
		newPlayer->nextPlayer         = NULL;

		// Generazione personaggio da assegnare
		do {
			// Genero una posizione casuale dall'array dei personaggi
			k = randRange(0, N_PERSONAGGI - 1);
			// Se personaggi[k] è uguale a emptyCharacter vuol dire che quel personaggio è già stato usato, quindi ne
			// genero un'altro, fino a quando non saranno diverso
		} while (strcmp(personaggi[k].name, emptyCharacter.name) == 0);

		newPlayer->character = personaggi[k]; // Assegno il personaggio dall'array
		personaggi[k] = emptyCharacter; // Sostiuisco il posto nell'array con il personaggio vuoto

		// Aggiungo l'elemento appena inizializzato alla lista dei giocatori
		playersList = addPlayerInTesta(playersList, newPlayer);

	}
	return playersList;
}

/**
 * checkUsernames() presi in input una username e la lista di giocatori e controlla che il nome utente non sia stato
 * ancora inserito
 * @param playersList Player *: lista dei giocatori già inseriti
 * @param username char *: nome utente nuovo giocatore
 * @return bool: true se la username è già stata utilizzata
 */
bool checkUsernames(Player *playersList, char *username) {
	bool check = false;
	while (playersList != NULL) {
		if (strcmp(playersList->username, username) == 0) {
			check = true;
		}
		playersList = playersList->nextPlayer;
	}
	return check;
}
/**
 * addCopyOfPlayerInCoda() è la subroutine che, presi in input una lista e un nuovo nodo, COPIA tale nodo alla coda di
 * tale lista, assicurandosi che il puntatore next di questo sia NULL
 * @param playerList Player *: lista alla quale aggiungere il nodo
 * @param playerToAdd Player *: nodo da aggiungere
 * @return Player *: testa della lista con il nodo aggiunto
 */
Player *addCopyOfPlayerInCoda(Player *playerList, Player *playerToAdd){
	Player *curr = playerList;

	// Se la lista è vuota
	if (playerList == NULL) {
		// Posso direttamente allocare il nuovo nodo sulla testa
		playerList = allocaGiocatore();
		// Lo copio sul nodo creato
		*playerList = *playerToAdd;
		// Il prossimo giocatore è settato a NULL
		playerList->nextPlayer = NULL;
	} else {
		// Altrimenti scorro fino all'ultimo nodo e lo aggiungo dopo questo
		while (curr->nextPlayer != NULL) {
			curr = curr->nextPlayer;
		}
		curr->nextPlayer = allocaGiocatore();
		// Lo copio sul nuovo nodo
		*(curr->nextPlayer) = *playerToAdd;
		// Setto il prox platyer a NILL
		curr->nextPlayer->nextPlayer = NULL;
	}
	return playerList;
}

// ============ ANDAMENTO PARTITA =============================================
/**
 * hasLost() è la subroutine che si occupa di controllare se un giocatore ha perso\n
 * CONDIZIONI DI SCONFITTA: si ha perso se si hanno \n
 * - tre carte ostacolo dello stesso tipo in mano, \n
 * - una carta di ogni tipo \n
 * NB: le carte ostacolo sono Jolly e valgono come qualunque altro tipo di carta, ma non contemporaneamente
 * @param pPlayer Player *: giocatore da analizzare
 * @return bool: true se il giocatore selezionato ha perso
 */
bool hasLost(Player *pPlayer) {
	CartaOstacolo *currOstacoli = pPlayer->listaCarteOstacolo;
	int numOstacoli[N_OSTACOLI] = {};
	bool lost = false;

	// Conteggio ostacoli per tipo
	while (currOstacoli != NULL) {
		numOstacoli[currOstacoli->type - 1]++;
		currOstacoli = currOstacoli->next;
	}

	// Se il numero di ostacoli di un singolo tipo è >= a 3 il giocatore ha perso
	for (int i = 0; i < N_OSTACOLI; ++i) {
		if (numOstacoli[i] >= 3) {
			lost = true;
		}
	}

	// Se non ha 3 ostacoli dello stesso tipo, posso continura il controllo
	if (lost != true) {
		// Se si hanno due carte esame, è sufficiente avere una sola carta di altro tipo per perdere
		if ((numOstacoli[ESAME-1] == 2) &&
		    (numOstacoli[STUDIO-1] > 0 || numOstacoli[SOPRAVVIVENZA-1] > 0 || numOstacoli[SOCIALE-1] > 0)) {
			lost = true;
		// Se si ha solo una carta esame
		} else if ((numOstacoli[ESAME-1] == 1) && (
		           // è sufficiente avere due carte diverse
		           (numOstacoli[STUDIO-1] > 0 && numOstacoli[SOPRAVVIVENZA-1] > 0)  ||
		            (numOstacoli[SOPRAVVIVENZA-1] > 0 && numOstacoli[SOCIALE-1] > 0) ||
		            (numOstacoli[STUDIO-1] > 0 && numOstacoli[SOCIALE-1] > 0)        ||
		            // O una coppia di carte uguali
		            numOstacoli[STUDIO-1] == 2 || numOstacoli[SOPRAVVIVENZA-1] == 2 || numOstacoli[SOCIALE-1] == 2)){
			lost = true;
			// Se non si hanno carte esame è necessario avere tre carte diverse
		} else if (numOstacoli[STUDIO-1] > 0 && numOstacoli[SOPRAVVIVENZA-1] > 0 && numOstacoli[SOCIALE-1]) {
			lost = true;
		}
	}
	return lost;
}

/**
 * hasWon() è la subroutine che controlla se il gioatore ha vinto la partita
 * CONDIZIONI DI VITTORIA: si vince se si hanno 60 CFU o si è l'ultimo giocatore rimasto
 * @param pPlayer Player *: giocatore da analizzare
 * @return bool: true se il giocatore ha vinto la bartita
 */
bool hasWon(Player *pPlayer) {
	bool won = false;

	if (pPlayer->cfu >= CFU_WINNER) {
		won = true;
	}

	return won;
}

/**
 * playerCheck() è la subroutine che controlla se un giocatore ha vinto o perso e effettua le operazioni dovute
 * @param nPlayers int: numero di giocatori in gioco
 * @param playersList Player **: doppio pt a lista gioatori da controllare
 * @param mazzoOstacoli CartaOstacolo **: mazzo carte ostacolo per mettere in fondo le carte di giocatori morti
 * @param mazzoScarti CarteCfu **: mazzo scarti carte cfu per scartare le carte di giocatori morti
 * @return bool: indica il termine della partita
 */
bool playerCheck(int *nPlayers, Player **playersList,
				 CartaOstacolo **mazzoOstacoli, CartaCfu **mazzoScarti) {
	Player *pPlayer = *playersList,
	       *prev    = NULL;
	bool end = false;

	while (pPlayer != NULL){
		// Se il giocatopre ha perso
		if (hasLost(pPlayer)) {
			printf("\n%s ha fatto la rinuncia agli studi\n", pPlayer->username);
			// Aggiungo i suoi ostacoli in coda al mazzo Ostacoli
			ostacoloInCoda(pPlayer->listaCarteOstacolo, mazzoOstacoli);
			// Scarto le sue carte on mano
			*mazzoScarti = scartaCarte(&pPlayer->manoCarteCfu, *mazzoScarti);

			// Isolo il suo puntatore
			if (prev == NULL) {
				*playersList = pPlayer->nextPlayer;
			} else {
				prev->nextPlayer = pPlayer->nextPlayer;
			}
			pPlayer->nextPlayer = NULL;
			// Dealloco la memoria connessa
			freeGiocatore(pPlayer);
			// Decremento il numero di giocatori
			*nPlayers --;

		// Se il fiocatore ha vinto o è l'ìunico rimasto
		} else if (hasWon(pPlayer)){
			printf("\n%s hai terminato il primo anno e vinto la partita\n"
			       "**************************************************************************\n"
			       "*   ____ ___  __  __ ____  _     ___ __  __ _____ _   _ _____ ___   _ _  *\n"
			       "*  / ___/ _ \\|  \\/  |  _ \\| |   |_ _|  \\/  | ____| \\ | |_   _|_ _| | | | *\n"
			       "* | |  | | | | |\\/| | |_) | |    | || |\\/| |  _| |  \\| | | |  | |  | | | *\n"
			       "* | |__| |_| | |  | |  __/| |___ | || |  | | |___| |\\  | | |  | |  |_|_| *\n"
			       "*  \\____\\___/|_|  |_|_|   |_____|___|_|  |_|_____|_| \\_| |_| |___| (_|_) *\n"
			       "*                                                                        *\n"
			       "**************************************************************************\n",
			       pPlayer->username);
			// La partita termina
			end = true;
		}

		// Se il giocatore corrente non ha ne vinto ne perso, passo al giocatore successivo
		prev = pPlayer;
		pPlayer = pPlayer->nextPlayer;
	}

	pPlayer = *playersList;
	// Effettuo il controllo di vittoria come ultimo giocatore al termine del ciclo per essere sicuro che venga
	// risulti sempre la vittoria eventuale anxhe se giocatori successivi a lui perdono
	if ((*playersList)->nextPlayer == NULL ){
				printf("\n%s sei rimasto l'unico tra i tuoi colleghi a non rinunciare agli studi e hai vinto la "
					   "partita\n"
			       "**************************************************************************\n"
			       "*   ____ ___  __  __ ____  _     ___ __  __ _____ _   _ _____ ___   _ _  *\n"
			       "*  / ___/ _ \\|  \\/  |  _ \\| |   |_ _|  \\/  | ____| \\ | |_   _|_ _| | | | *\n"
			       "* | |  | | | | |\\/| | |_) | |    | || |\\/| |  _| |  \\| | | |  | |  | | | *\n"
			       "* | |__| |_| | |  | |  __/| |___ | || |  | | |___| |\\  | | |  | |  |_|_| *\n"
			       "*  \\____\\___/|_|  |_|_|   |_____|___|_|  |_|_____|_| \\_| |_| |___| (_|_) *\n"
			       "*                                                                        *\n"
			       "**************************************************************************\n",
			       pPlayer->username);
			// La partita termina
			end = true;
	}
	return end;
}