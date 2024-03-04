//
// Created by Monti on 03/03/24.
//

#include "starting.h"
#include "giocatori.h"
#include "mazzoCfu.h"
#include "carteOstacolo.h"

FILE *startGame(char *saveName, FILE *fSave,
		  Character charactersArr[], int *nPlayers, Player **playerList,
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

	do {
		printf("[%d] Inizia una nuova partita\n"
		       "[%d] Carica una partita esistente\n"
		       "[%d] Esci dal gioco\n"
		       ">>> ",
		       NEW_GAME, LOAD_GAME, LEAVE);

		scanf("%d", &choice);

		if (choice < LEAVE || choice > LOAD_GAME) {
			printf("\nInput non valido, riprova\n");
		}
	} while (choice < LEAVE || choice > LOAD_GAME);

	switch (choice) {
		case NEW_GAME:
			fSave = newGame(saveName, fSave,
							charactersArr, nPlayers, playerList,
							mazzoCfu, mazzoScarti,
							mazzoOstacoli);
			break;
		case LOAD_GAME:
			fSave = loadGame(saveName, fSave,
			                 charactersArr, nPlayers, playerList,
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

	return fSave;
}

FILE *newGame(char *saveName, FILE *fSave,
			  Character charactersArr[], int *nPlayers, Player **playerList,
              CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			  CartaOstacolo **mazzoOstacoli) {
	printf("\nInserisci un nome per il salvataggio\n"
		   "(Non includere estensione .sav, maxlen: 31): ");
	scanf(" %[^\n]31s", saveName);
	strcat(saveName, SAVES_EXTENSION);

	printf("\nCreazione di un nuovo salvataggio in corso...\n");
	fSave = openFile(saveName, BIN_WRITE);

	initNewGame(charactersArr, nPlayers, playerList,
				mazzoCfu, mazzoScarti,
				mazzoOstacoli);

	saveOnFile();
	return fSave;
}

void initNewGame(Character charactersArr[], int *nPlayers, Player **playerList,
                  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                  CartaOstacolo **mazzoOstacoli) {
	FILE *fPersonaggi = NULL,
		 *fCfu        = NULL,
		 *fOstacoli   = NULL;

	fPersonaggi = openFile(FILE_PERSONAGGI, READ);
	fCfu        = openFile(FILE_CARTE_CFU, READ);
	fOstacoli   = openFile(FILE_OSTACOLI, READ);

	// Creazione array charactersArr
	parseCharacters(fPersonaggi, charactersArr);

	// Lettura carte e creazione mazzo mischiato
	*mazzoCfu      = creaMazzoCfu(fCfu);
	*mazzoCfu      = mescolaMazzo(mazzoCfu);
	*mazzoOstacoli = creaMazzoOstacoli(fOstacoli);

	// Chiusura file
	fclose(fPersonaggi);
	fclose(fOstacoli);
	fclose(fCfu);

	// Giocatori
	*nPlayers   = acquisisciNumGiocatori();
	*playerList = initGiocatori(*nPlayers, mazzoCfu, charactersArr, mazzoScarti);
}

FILE *loadGame(char *saveName, FILE *fSave,
			   Character charactersArr[], int *nPlayers, Player **playerList,
               CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
               CartaOstacolo **mazzoOstacoli){
	printf("\nInserisci il nome del salvataggio che vuoi caricare\n"
		   "(Non includere estensione .sav): ");
	scanf(" %[^\n]31s", saveName);
	strcat(saveName, SAVES_EXTENSION);

	printf("\nCaricamento salvataggio esistente in corso");
	fSave = openFile(saveName, BIN_READ);

	loadFromFile(fSave,
				 charactersArr, nPlayers, playerList,
				 mazzoCfu, mazzoScarti,
				 mazzoOstacoli);

	return fSave;
}