//
// Created by Monti on 28/02/24.
//

#include "saves.h"
#include "memoria.h"
#include "mazzoCfu.h"
#include "carteOstacolo.h"

/**
 * saveOnFile() è la subroutine per salvare lo stato attuale della partita in un file binario
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param charactersArr Characters[4]: array delle strutture personaggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playersList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void saveOnFile(char *saveName, FILE *fSave,
                Character charactersArr[], int *nPlayers, Player *playersList,
                CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
                CartaOstacolo *mazzoOstacoli) {

	int nOstacoli               = 0,    // Numero di carte ostacolo da scrivere
		nCfu                    = 0;    // Numero di carte cfu da scrivere

	rewind(fSave); // Mi assicuro di essere all'inizio del file di salvataggio
	printf("Salvataggio su %s in corso...", saveName);

	// Scrittura numero Giocatori
	fwrite(nPlayers, sizeof(int), SAVES_UNIT, fSave);
	// Scrittura Giocatori
	writePlayersList(fSave, *nPlayers, playersList);

	// Scrittura carte Cfu mazzo
	nCfu    = contaCarteCfu(mazzoCfu);
	fwrite(&nCfu, sizeof(int), SAVES_UNIT, fSave);
	writeCfuList(fSave, nCfu, mazzoCfu);

	// Scrittura carte Cfu scarti
	nCfu    = contaCarteCfu(mazzoScarti);
	fwrite(&nCfu, sizeof(int), SAVES_UNIT, fSave);
	writeCfuList(fSave, nCfu, mazzoScarti);

	// Scrittura carte Ostacolo mazzo
	nOstacoli    = contaOstacoli(mazzoOstacoli);
	fwrite(&nOstacoli, sizeof(int), SAVES_UNIT, fSave);
	writeOstacoliList(fSave, nOstacoli, mazzoOstacoli);

	if (DBG) printf("\nDBG: save complete");
}

/**
 * writePlayersList() è la subroutine per scrivere i nodi di una lista di Player lunga n
 * @param fSave FILE *: puntatore al file di salvataggio dove scrivere
 * @param n int: numero di giocatori da scrivere
 * @param playersList Player *: puntatore alla testa della lista da scrivere
 */
void writePlayersList(FILE *fSave, int n, Player *playersList) {
	Player *currPlayer = playersList;
	int nOstacoli = -1;

	for (int i = 0; i < n; ++i) {
		fwrite(currPlayer, sizeof(Player), SAVES_UNIT, fSave);

		// Scrittura carte Cfu Mano
		writeCfuList(fSave, CARTE_PER_MANO, currPlayer->manoCarteCfu);

		// Scrittura numero carte Ostacolo
		nOstacoli = contaOstacoli(currPlayer->listaCarteOstacolo);
		fwrite(&nOstacoli, sizeof(int), SAVES_UNIT, fSave);

		// Scrittura carte Ostacolo
		writeOstacoliList(fSave, nOstacoli, currPlayer->listaCarteOstacolo);

		if (DBG) printf("\nDBG: saved %s player", currPlayer->username);
		// Scorrimento giocatori
		currPlayer  = currPlayer->nextPlayer;
	}
	if (DBG) printf("\nDBG: saved %d players", n);
}

/**
 * writeCfuList() è la subroutine per scrivere i nodi di una lista di CarteCfu lunga n
 * @param fSave FILE *: puntatore al file di salvataggio dove scrivere
 * @param n int: numero di carte da scrivere
 * @param cfuList CartaCfu *: puntatore alla testa della lista da scrivere
 */
void writeCfuList(FILE *fSave, int n, CartaCfu *cfuList) {
	CartaCfu *curr = cfuList;
	for (int j = 0; j < n; ++j) {
		fwrite(curr, sizeof(CartaCfu), SAVES_UNIT, fSave);
		if (curr->next != NULL) {
			curr = curr->next;
		}
	}
	if (DBG) printf("\nDBG: saved %d carteCfu", n);
}

/**
 * writeOstacoliList() è la subroutine per scrivere i nodi di una lista di CarteOstacoli lunga n
 * @param fSave FILE *: puntatore al file di salvataggio dove scrivere
 * @param n int: numero di carte da scrivere
 * @param ostacoliList CartaOstacoli *: puntatore alla testa della lista da scrivere
 */
void writeOstacoliList(FILE *fSave, int n, CartaOstacolo *ostacoliList) {
	CartaOstacolo *curr = ostacoliList;
	for (int j = 0; j < n; ++j) {
		fwrite(curr, sizeof(CartaOstacolo), SAVES_UNIT, fSave);
		if (curr->next != NULL) {
			curr = curr->next;
		}
	}
	if (DBG) printf("\nDBG: saved %d carteOstacolo", n);
}

/**
 * loadSaveFromFile() è la subroutine che legge il file di salvataggio e ne carica i contenuti
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param charactersArr Characters[4]: array delle strutture personaggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void loadSaveFromFile(char *saveName, FILE *fSave,
                      Character charactersArr[], int *nPlayers, Player **playerList,
                      CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                      CartaOstacolo **mazzoOstacoli) {

	int check          = 0,
	    nManoOstacoli  = 0,
	    nCfuMazzo      = 0,
	    nCfuScarti     = 0,
	    nMazzoOstacoli = 0;


	Player playerAux,
	       *playerHead = *playerList;

	CartaCfu carteAux,
	         *pNewCarta      = NULL,
	         *headCarteMano  = NULL,
	         *headCarteMazzo = NULL;

	CartaOstacolo ostacoloAux,
				  *pNewOstacolo      = NULL,
	              *headManoOstacoli  = NULL,
	              *headMazzoOstacoli = NULL;

	rewind(fSave);
	printf("\nLettura da %s in corso...", saveName);

	// Lettura numero Players
	check = fread(nPlayers, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_PLAYERS);
	}

	// Lettura Players
	for (int i = 0; i < *nPlayers; ++i) {
		check = 0;
		check = fread(&playerAux, sizeof(Player), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_PLAYER);
		}
		// Creazione lista dei giocatori
		if (*playerList == NULL) {
			playerHead = allocaGiocatore();
			*playerHead = playerAux;
			*playerList = playerHead;
		} else {
			playerHead->nextPlayer = allocaGiocatore();
			*playerHead->nextPlayer = playerAux;
			playerHead = playerHead->nextPlayer;
		}

		// Lettura mano Cfu
		playerHead->manoCarteCfu = NULL;
		headCarteMano = playerHead->manoCarteCfu;
		for (int j = 0; j < CARTE_PER_MANO; ++j) {
			check = 0;
			check = fread(&carteAux, sizeof(CartaCfu), SAVES_UNIT, fSave);
			if (check == 0) {
				exit(ERR_READ_SAVE_MANO_CFU);
			}
			pNewCarta = allocaCartaCfu();
			*pNewCarta = carteAux;
			pNewCarta->next = NULL;
			cartaCfuInCoda(&playerHead->manoCarteCfu, pNewCarta);
		}

		// Lettura carte Ostacolo
		playerHead->listaCarteOstacolo = NULL;
		headManoOstacoli = playerHead->listaCarteOstacolo;
		check            = 0;
		check            = fread(&nManoOstacoli, sizeof(int), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_N_OSTACOLI);
		}
		for (int j = 0; j < nManoOstacoli; ++j) {
			check = 0;
			check = fread(&ostacoloAux, sizeof(CartaOstacolo), SAVES_UNIT, fSave);
			if (check == 0) {
				exit(ERR_READ_SAVE_OSTACOLI);
			}
			pNewOstacolo = allocaCartaOstacolo();
			*pNewOstacolo = ostacoloAux;
			pNewOstacolo->next = NULL;
			ostacoloInCoda(pNewOstacolo, &(playerHead->listaCarteOstacolo));
		}
	}

	// Lettura numero carte Cfu mazzo
	check = 0;
	check = fread(&nCfuMazzo, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_CFU);
	}

	// Lettura carte Cfu
	for (int i = 0; i < nCfuMazzo; ++i) {
		check = 0;
		check = fread(&carteAux, sizeof(CartaCfu), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_MAZZO_CFU);
		}
		pNewCarta = allocaCartaCfu(); // Alloca nuova carta e salva in var
		*pNewCarta = carteAux; // Inizializza spazio allocato con dati letti
		pNewCarta->next = NULL;
		cartaCfuInCoda(mazzoCfu, pNewCarta);
	}

	// Lettura numero carte Cfu scarti
	check = 0;
	check = fread(&nCfuScarti, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_SCARTI);
	}

	// Lettura carte Cfu scrti
	for (int i = 0; i < nCfuScarti; ++i) {
		check = 0;
		check = fread(&carteAux, sizeof(CartaCfu), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_MAZZO_SCARTI);
		}

		pNewCarta = allocaCartaCfu(); // Alloca nuova carta e salva in var
		*pNewCarta = carteAux; // Inizializza spazio allocato con dati letti
		pNewCarta->next = NULL;
		cartaCfuInCoda(mazzoScarti, pNewCarta);
	}

	// Lettura numero carte Ostacolo mazzo
	check = 0;
	check = fread(&nMazzoOstacoli, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_OSTACOLI);
	}
	// Lettura carte Ostacolo mazzo
	for (int j = 0; j < nManoOstacoli; ++j) {
		check = 0;
		check = fread(&ostacoloAux, sizeof(CartaOstacolo), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_MAZZO_OSTACOLI);
		}
		pNewOstacolo = allocaCartaOstacolo();
		*pNewOstacolo = ostacoloAux;
		pNewOstacolo->next = NULL;
		ostacoloInCoda(pNewOstacolo, mazzoOstacoli);
	}

}

void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta) {
	fprintf(fLog, "TURNO %d: %s gioca %s\n", numTurno, user, carta);
}

