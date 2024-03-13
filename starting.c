#include "starting.h"
#include "giocatori.h"
#include "mazzoCfu.h"
#include "carteOstacolo.h"

/**
 * startGame() è la subroutine per controllare l'inizio del programma e gestire la creazione di una nuova partita o il
 * caricamento di un salvataggio esistente
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione .sav + '/0'
 * @param charactersArr Characters[4]: array delle strutture personaggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void startGame(char *saveName,
				Character charactersArr[],
				int *nPlayers, Player **playerList,
				CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
				CartaOstacolo **mazzoOstacoli) {
	int choice;

	printf("\nWelcome to...\n"
		   " _   _                           _     _ _   _   _        _____ _             _            _       \n"
	       "| | | |                         | |   (_) | | | | |      /  ___| |           | |          | |      \n"
	       "| |_| | __ _ _ __  _ __  _   _  | |    _| |_| |_| | ___  \\ `--.| |_ _   _  __| | ___ _ __ | |_ ___ \n"
	       "|  _  |/ _` | '_ \\| '_ \\| | | | | |   | | __| __| |/ _ \\  `--. \\ __| | | |/ _` |/ _ \\ '_ \\| __/ __|\n"
	       "| | | | (_| | |_) | |_) | |_| | | |___| | |_| |_| |  __/ /\\__/ / |_| |_| | (_| |  __/ | | | |_\\__ \\\n"
	       "\\_| |_/\\__,_| .__/| .__/ \\__, | \\_____/_|\\__|\\__|_|\\___| \\____/ \\__|\\__,_|\\__,_|\\___|_| |_|\\__|___/\n"
	       "            | |   | |     __/ |                                                                    \n"
	       "            |_|   |_|    |___/                                                                     \n");

	// Menu di inizio partita: permette di scegliere se iniziare una partita da zero o caricare un salvataggio
	choice = mainMenu();

	// Esecuzione della scelta di inizio partita
	switch (choice) {
		case NEW_GAME:
			newGame(saveName,
					charactersArr, nPlayers, playerList,
			        mazzoCfu, mazzoScarti,
					mazzoOstacoli);
			break;

		case LOAD_GAME:
			loadGame(saveName, nPlayers, playerList,
			         mazzoCfu, mazzoScarti,
			         mazzoOstacoli);
			break;
		case LEAVE:
			printf("\nStai lasciato il gioco");
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("\nErrore\n");
			exit(EXIT_FAILURE);
			break;
	}
}

/**
 * mainMenu() è la subroutine che controlla il menu di inizio partita: permette di scegliere se\n
 * 	* iniziare una partita da zero e creare un nuovo salvataggio\n
 * 	* continuare una partita esistente caricandola da un file binario di salvataggio
 * @return int: la scelta fatta dall'utente
 */
int mainMenu() {
	int choice = -1;
	printf("[%d] Inizia una nuova partita\n"
	       "[%d] Carica una partita esistente\n"
	       "[%d] Esci dal gioco\n",
	       NEW_GAME, LOAD_GAME, LEAVE);

	// Richiedo l'input dell'utente
	do {
		printf(">>> ");
		scanf("%d", &choice);

		// Chiedo all'utente di reinserire l'input se questo risulta errato
		if (choice < LEAVE || choice > LOAD_GAME) {
			printf("\nInput non valido, riprovare\n");
		}
	} while (choice < LEAVE || choice > LOAD_GAME);

	return choice;
}

/**
 * newGame() è la subroutine che si occupa di inizializzare una nuova partita:\n
 * 	- crea un file di salvataggio\n
 * 	- legge i personaggi\n
 * 	- legge i mazzi di carte\n
 * 	- crea la lista di giocatori\n
 * 	- distribuisce la prima mano di carte ai giocatori\n
 * 	- effettua il primo salvataggio\n
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param charactersArr Characters[4]: array delle strutture personaggio
 * @param nPlayers int *: puntatore al numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void newGame(char *saveName,
			 Character charactersArr[],
			 int *nPlayers, Player **playerList,
			 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			 CartaOstacolo **mazzoOstacoli) {
	FILE *fSave = NULL;

	// Lettura nome save
	printf("\nInserisci un nome per il salvataggio\n"
		   "(Non includere estensione .sav, maxlen: 31): ");
	scanf(" %[^\n]31s", saveName);
	strcat(saveName, SAVES_EXTENSION); // Aggiunta estensione al nome del save

	// Creazione file di salvataggio
	printf("\nCreazione di un nuovo salvataggio in corso...\n");
	fSave = openFile(saveName, BIN_WRITE);

	// Inizializzazione partita
	initNewGame(charactersArr, nPlayers, playerList,
				mazzoCfu, mazzoScarti,
				mazzoOstacoli);

	// Salvataggio sul file
	saveOnFile(saveName, fSave, nPlayers, *playerList,
	           *mazzoCfu, *mazzoScarti,
	           *mazzoOstacoli);

	fclose(fSave);
}

/**
 * initNewGame() è la subroutine per inizializzare la nuova partita:\n
 *  - legge i file personaggi e carte \n
 *  - crea i mazzi delle carte \n
 *  - mescola i mazzi creati
 * @param charactersArr Characters[4]: array delle strutture personaggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void initNewGame(Character charactersArr[],
				 int *nPlayers, Player **playerList,
				 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                 CartaOstacolo **mazzoOstacoli) {
	FILE *fPersonaggi = NULL,
		 *fCfu        = NULL,
		 *fOstacoli   = NULL;

	// Apertura file
	fPersonaggi = openFile(FILE_PERSONAGGI, READ);
	fCfu        = openFile(FILE_CARTE_CFU, READ);
	fOstacoli   = openFile(FILE_OSTACOLI, READ);

	// Creazione array charactersArr
	parseCharacters(fPersonaggi, charactersArr);

	// Lettura carte e creazione mazzo mischiato
	*mazzoCfu      = creaMazzoCfu(fCfu);
	*mazzoCfu      = mescolaMazzo(mazzoCfu);
	*mazzoOstacoli = creaMazzoOstacoli(fOstacoli);
	*mazzoOstacoli = mescolaMazzoOstacoli(mazzoOstacoli);

	// Chiusura file
	fclose(fPersonaggi);
	fclose(fOstacoli);
	fclose(fCfu);

	// Giocatori
	*nPlayers   = acquisisciNumGiocatori();
	*playerList = initGiocatori(*nPlayers, charactersArr, mazzoCfu, mazzoScarti);
}

/**
 * loadGame() è la subroutine che si occupa di caricare da un salvataggio una partita già iniziata:\n
 * 	- carica i giocatori con personaggi, carte in mano e ostacoli\n
 * 	- carica i mazzi di carte\n
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param fSave FILE *: puntatore al file da cui bisogna caricare
 * @param nPlayers int *: puntatore al numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void loadGame(char *saveName, int *nPlayers,
			  Player **playerList,
			  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
              CartaOstacolo **mazzoOstacoli) {
	FILE *fSave = NULL;
	// Input nome salvataggio
	printf("\nInserisci il nome del salvataggio che vuoi caricare\n"
		   "(Non includere estensione .sav): ");
	scanf(" %[^\n]31s", saveName);
	strcat(saveName, SAVES_EXTENSION);

	printf("\nCaricamento salvataggio esistente in corso");
	fSave = openFile(saveName, BIN_READ);

	// Caricamento salvataggio
	loadSaveFromFile(saveName, fSave, nPlayers, playerList,
	                 mazzoCfu, mazzoScarti,
	                 mazzoOstacoli);

	// Chiusura file di salvataggio aperto in lettura
	fclose(fSave);
}