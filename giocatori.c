//
// Created by Monti on 07/02/24.
//

#include "giocatori.h"
#include "carteOstacolo.h"
#include "memoria.h"
#include "turno.h"
#include "mazzoCfu.h"

// ============ PERSONAGGI =====================================================
/**
 * parseCharacters() è la funzione che legge il file dei personaggi e conserva le informazioni di ogni personaggio in
 * un'array di sturtture personaggio
 *
 * Una struttura characters nel campo bonusMalus abbiamo un array dove vengono salvati gli eventuali bonus e malus di
 * ogni giocatore per ogni tipologia di ostacolo
 *
 * @param fp è il file da leggere [personaggi.txt]
 * @param characters è l'indirizzo dell'array di strutture personaggio
*/
void parseCharacters(FILE *fp, Character characters[]) {
	// Per ogni personaggio
	for (int i = 0; i < N_PERSONAGGI; i++){
		// Per ogni ostacolo
		for (int j = 0; j < N_OSTACOLI; j++){
			fscanf(fp, "%d ", &characters[i].bonusMalus[j]);
		}
		fscanf(fp, "%[^\n]31s", characters[i].name);
		fscanf(fp, "\n");
	}
}

/**
 * printBonusMalus() è la subroutine che stampa i bonus e i malus di un personaggio
 * @param arr è l'array corrispondente a ciascun personaggio
 */
void printBonusMalus(int arr[N_OSTACOLI]){
	char *types[] = {"Studio", "Sopravvivenza", "Sociale", "Esame"};

	printf("Bonus e Malus:\n");
	for (int j = 0; j < N_OSTACOLI; ++j) {
		printf("|\t%13s: %d\n", types[j], arr[j]);
	}
}

void printCharacter(Character *pCharacter){
	printf("Personaggio: %s\n"
	       "|\t", pCharacter->name);
	printBonusMalus(pCharacter->bonusMalus);

}

// ============ IO ============================================================

/**
 * acquisisciNumGiocatore() è la funzione che acquisisce il numero dei partecipanti alla partita
 * La funzione si occupa di controllare che l'input sia valido e invita a ritentare in caso di input non valido
 * @return il numero di giocatori alla partita
 */
int acquisisciNumGiocatori() {
	int nGiocatori;

	do {
		printf("Quanti giocatori parteciperanno oggi? [2-4] ");
		scanf(" %d", &nGiocatori);

		if (nGiocatori < 2 || nGiocatori > 4){
			printf("Il numero di giocatori deve essere compreso tra 2 e 4:\n"
			       "\t controlla l'input o organizzatevi in squadre :)\n");
		}
	} while (nGiocatori < 2 || nGiocatori > 4);

	return nGiocatori;
}

void printGiocatore(Player *pPlayer) {
	printf("\nGiocatore:\n");
	printf("|\t Username: %s\n"
	       "|\t Cfu: %d\n"
	       "|\t ",
	       pPlayer->username, pPlayer->cfu);

	printCharacter(&pPlayer->character);

	if (pPlayer->listaCarteOstacolo != NULL) {
		printf("\nCarte ostacolo:");
		printOstacoli(pPlayer->listaCarteOstacolo);
	}
	printf("+----------------------------------------\n");
}


/**
 * printGiocatori() è la subroutine che stampa la lista dei giocatori, il loro nome, personaggio, punteggio e mano
 * @param listaGiocatori è la lista di giocatori da stampare
 */
void printGiocatori(Player *listaGiocatori, bool stampaCarte) {
	Player *head = listaGiocatori;
	printf("\n=== GIOCATORI ===\n");

	// Fino a quando head indica un giocatore valido
	for (int i = 0; head != NULL; ++i) {
		// Stampa le informazioni
		printGiocatore(head);
		// Scorri al giocatore successivo
		head = head->nextPlayer;
	}
}

// ============ LIST MANAGEMENT ===============================================

/**
 * initGiocatori() è la funzione che, dati il numero dei giocatori e il mazzo di carte Cfu, restituisce una lista di
 * giocatori, occupandosi di inizializzarla, assegnando un username, personaggio e una mano di cinque carte iniziali
 * @param nGiocatori è il numero di giocatori che partecipano alla partita
 * @param mazzoCfu è un puntatore alla testa della lista che rappresenta il mazzo delle carte Cfu, passata alla
 * funzione per permettere l'assegnamento della mano iniziale di carte
 * @param personaggi è l'array delle strutture dei personaggi
 * @return la funzione restituisce listaGiocatori, una lista di strutture di tipo giocatore
 */
Player *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu, Character personaggi[], CartaCfu **mazzoScarti)  {
	Player    *listaGiocatori = NULL,
	          *head           = NULL,
	          aux;
	int       k; // Indice per l'accesso all'array di personaggi
	Character emptyCharacter  = {}; // Struttura personaggio ausiliaria vuota

	printf("\n=== PARTECIPANTI ===\n");
	// Per ogni giocatore
	for (int i = 0; i < nGiocatori; ++i) {
		// Richiedi username
		printf("\nGIOCATORE %d\n", i);
		printf("Inserire username: ");
		scanf(" %[^\n]31s", aux.username);

		// Inizializza la struttura
		aux.cfu           = 0; // Punteggio di partenza
		aux.manoCarteCfu  = NULL;
		aux.manoCarteCfu  = distribuisciCarte(aux.manoCarteCfu, mazzoCfu, mazzoScarti); // Mano iniziale
		aux.listaCarteOstacolo = NULL;
		aux.nextPlayer         = NULL;

		do {
			// Genero una posizione casuale dall'array dei personaggi
			k = randRange(0, N_PERSONAGGI - 1);
			// Se personaggi[k] è uguale a emptyCharacter vuol dire che quel personaggio è già stato usato, quindi ne
			// genero un'altro, fino a quando non saranno diverso
		} while (strcmp(personaggi[k].name, emptyCharacter.name) == 0);

		aux.character = personaggi[k]; // Assegno il personaggio dall'array
		personaggi[k] = emptyCharacter; // Sostiuisco il posto nell'array con il personaggio vuoto


		// Creazione lista dei giocatori
		if (listaGiocatori == NULL) {
			listaGiocatori = allocaGiocatore();
			*listaGiocatori = aux;
			head = listaGiocatori;
		} else {
			head->nextPlayer = allocaGiocatore();
			*head->nextPlayer = aux;
			head = head->nextPlayer;
		}

	}
	return listaGiocatori;
}

/**
 * addPlayerInCoda() è la subroutine che, presi in input una lista e un nuovo nodo, COPIA tale nodo alla coda di
 * tale lista, assicurandosi che il puntatore next di questo sia NULL
 * @param playerList è la lista alla quale aggiungere il nodo
 * @param playerToAdd è il nodo da aggiungere
 * @return la testa della lista con il nodo aggiunto
 */
Player *addPlayerInCoda(Player *playerList, Player *playerToAdd){

	Player *head = playerList,
	       aux = *playerToAdd;
	aux.nextPlayer = NULL;

	// Se la lista è vuota
	if (playerList == NULL) {
		// Posso direttamente allocare il nuovo nodo sulla testa
		playerList = allocaGiocatore();
		*playerList = aux;
	} else {
		// Altrimenti scorro fino all'ultimo nodo e lo aggiungo dopo questo
		while (head->nextPlayer != NULL) {
			head = head->nextPlayer;
		}
		head->nextPlayer = allocaGiocatore();
		*(head->nextPlayer) = aux;
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
 * @param pPlayer è il giocatore da analizzare
 * @return booleano lost: true se il giocatore selezionato ha perso
 */
bool hasLost(Player *pPlayer) {
	CartaOstacolo *headOstacoli = pPlayer->listaCarteOstacolo;
	int numOstacoli[N_OSTACOLI] = {};
	bool lost = false;

	while (headOstacoli != NULL) {
		numOstacoli[headOstacoli->type - 1]++;
		headOstacoli = headOstacoli->next;
	}

	for (int i = 0; i < N_OSTACOLI; ++i) {
		if (numOstacoli[i] == 3) {
			lost = true;
		}
	}

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
 * @param pPlayer è il giocatore da analizzare
 * @return booleano: true se il giocatore ha vinto la bartita
 */
bool hasWon(Player *pPlayer) {
	bool won = false;

	if (pPlayer->cfu >= CFU_WINNER) {
		won = true;
	}

	return won;
}

/**
 * playerCheck() è la subroutine che controlla se un giocatore ha vinto o perso la partita e effettua le operazioni
 * dovute
 * @param playersList è la lista gioatori da controllare
 * @param mazzoOstacoli è il mazzo delle carte ostacolo per mettere in fondo le carte di eventuali giocatori morti
 * @param mazzoScarti è il mazzo scarti delle carte cfu per scartare le carte dei giocatori morti
 * @param nPlayers è il numero di giocatori in gioco
 * @return un booleano che indica il termine della partita
 */
bool playerCheck(Player **playersList, CartaOstacolo **mazzoOstacoli, CartaCfu **mazzoScarti, int *nPlayers) {
	Player *pPlayer = *playersList,
	       *prev    = NULL;
	bool end = false;

	while (pPlayer != NULL){
		if (hasLost(pPlayer)) {
			printf("\n%s ha fatto la rinuncia agli studi\n", pPlayer->username);
			ostacoloInCoda(&pPlayer->listaCarteOstacolo, mazzoOstacoli);
			scartaCarte(&pPlayer->manoCarteCfu, mazzoScarti);
			if (prev == NULL){
				*playersList = pPlayer->nextPlayer;
			} else {
				prev->nextPlayer = pPlayer->nextPlayer;
			}
			pPlayer->nextPlayer = NULL;
			freeGiocatore(pPlayer);
			*nPlayers --;

		} else if (hasWon(pPlayer) || (*playersList)->nextPlayer == NULL ){
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
			end = true;
		}
		prev = pPlayer;
		pPlayer = pPlayer->nextPlayer;
	}
	return end;
}