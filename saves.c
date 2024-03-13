#include "saves.h"
#include "memoria.h"
#include "mazzoCfu.h"
#include "carteOstacolo.h"
#include "giocatori.h"

// ========== SALVATAGGIO ==============================================================================================
/**
 * saveOnFile() è la subroutine per salvare lo stato attuale della partita in un file binario
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playersList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void saveOnFile(char *saveName, FILE *fSave,
		   int *nPlayers, Player *playersList,
		   CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
           CartaOstacolo *mazzoOstacoli) {

	int nOstacoli               = 0,    // Numero di carte ostacolo da scrivere
		nCfu                    = 0;    // Numero di carte cfu da scrivere

	rewind(fSave); // Mi assicuro di essere all'inizio del file di salvataggio
	printf("Salvataggio su %s in corso...", saveName);

	// Scrittura Giocatori
	fwrite(nPlayers, sizeof(int), SAVES_UNIT, fSave);
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

// ========== CARICAMENTO ==============================================================================================
/**
 * loadSaveFromFile() è la subroutine che legge il file di salvataggio e ne carica i contenuti
 * @param saveName string[31 + 4 + 1]: nome del salvataggio + estensione '.sav' + '/0'
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param nPlayers int: numero di giocatori nella partita
 * @param playerList Player **: doppio puntatore alla testa della lista di giocatori in partita
 * @param mazzoCfu CartaCfu **: doppio puntatore alla testa del mazzo delle carte Cfu
 * @param mazzoScarti CartaCfu **: doppio puntatore alla testa del mazzo di scarti delle carte Cfu
 * @param mazzoOstacoli CartaOstacolo **: doppio puntatore alla testa del mazzo delle carte Ostacolo
 */
void loadSaveFromFile(char *saveName, FILE *fSave,
					  int *nPlayers, Player **playerList,
					  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
					  CartaOstacolo **mazzoOstacoli) {

	int check          = 0,
	    nCfuMazzo      = 0,
	    nCfuScarti     = 0,
	    nMazzoOstacoli = 0;

	rewind(fSave);
	printf("\nLettura da %s in corso...", saveName);

	// Lettura Players
	check = fread(nPlayers, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_PLAYERS);
	}
	*playerList = readPlayersList(fSave, *nPlayers);


	// Lettura carte Cfu mazzo
	check = 0;
	check = fread(&nCfuMazzo, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_CFU);
	}
	*mazzoCfu = readCfuList(fSave, nCfuMazzo);

	// Lettura carte Cfu scarti
	check = 0;
	check = fread(&nCfuScarti, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_SCARTI);
	}
	*mazzoScarti = readCfuList(fSave, nCfuScarti);

	// Lettura carte Ostacolo mazzo
	check = 0;
	check = fread(&nMazzoOstacoli, sizeof(int), SAVES_UNIT, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_OSTACOLI);
	}
	*mazzoOstacoli = readOstacoliList(fSave, nMazzoOstacoli);

}

/**
 * readPlayersList() è la subroutine che legge dal file di salvataggio la lista dei giocatori, comprese le carte cfu
 * e ostacolo di ciascuno
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param nPlayers int: numero di player da leggere
 * @param playersList Player *: puntatore a lla lista dei giocatori
 * @return Player *: puntatore alla lista dei giocatori riempita
 */
Player *readPlayersList(FILE *fSave, int nPlayers) {
	int check         = 0,  // Check di lettura con successo
		nManoOstacoli = 0;  // Carte ostacoli in mano al giocatore
	Player *newPlayer   = NULL, // Puntatore a un nuovo giocatore
		   *playersList = NULL; // Puntatore alla lista dei giocatori

	// Ciclo sul numero dei giocatori
	for (int i = 0; i < nPlayers; ++i) {
		newPlayer = allocaGiocatore();
		// Leggo un giocatore sulla variabile appena allocata
		check = 0;
		check = fread(newPlayer, sizeof(Player), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_PLAYER);
		}

		// LEggo le carte della mano del giocatore
		newPlayer->manoCarteCfu = readCfuList(fSave, CARTE_PER_MANO);

		// Leggo il numero di carte ostacolo che ha il giocatore
		check = 0;
		check = fread(&nManoOstacoli, sizeof(int), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_N_OSTACOLI);
		}

		// Leggo le carte ostacolo del giocatore
		newPlayer->listaCarteOstacolo = readOstacoliList(fSave, nManoOstacoli);
		// Aggiungo il giocatore alla lista
		playersList = addPlayerInTesta(playersList, newPlayer);

		if (DBG) printf("\nDBG: read %s player", newPlayer->username);
	}

	if (DBG) printf("\nDBG: read %d players", nPlayers);
	return playersList;
}

/**
 * readCfuList() è la subroutine che legge dal file di salvataggio una lista di carteCfu
 * @param fSave FILE *: puntatore al file di salvataggio
 * @param n int: numero di carte da leggere
 * @return CartaCfu *: puntatore a lista di Carte Cfu
 */
CartaCfu *readCfuList(FILE *fSave, int n) {
	int check = 0; // Controllo lettura con successo
	CartaCfu *newCarta = NULL, // Puntatore alla carta da leggere
			 *cfuList  = NULL; // Lista di carte

	// Ciclo sul numero di carte
	for (int j = 0; j < n; ++j) {
		newCarta = allocaCartaCfu();
		// Leggo la carta nel puntatore appena allocata
		check = 0;
		check = fread(newCarta, sizeof(CartaCfu), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_MANO_CFU);
		}
		// Aggiungo la carta alla lista da restituire
		cfuList = cartaCfuInTesta(cfuList, newCarta);
	}
	if (DBG) printf("\nDBG: read %d cfuCards", n);
	return cfuList;
}

/**
 * readOstacoliList() è la subroutine per leggere una lista di ostacoli dal file di salvataggio
 * @param fSave FIEL *: puntatore al file di salvataggio dal qiale leggere
 * @param n int: numero di carte da leggere
 * @return CartaOstacolo *: puntatore alla lista di carte lette
 */
CartaOstacolo *readOstacoliList(FILE *fSave, int n) {
	int check = 0; // Controlo lettura con successo
	CartaOstacolo *newOstacolo  = NULL, // Puntatore alla carta da leggere
				  *ostacoliList = NULL; // Lista di carte

	// Ciclo sil numero di carte
	for (int j = 0; j < n; ++j) {
		newOstacolo = allocaCartaOstacolo();
		check = 0;
		// Leggo la carta nel puntatore appena allocato
		check = fread(newOstacolo, sizeof(CartaOstacolo), SAVES_UNIT, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_OSTACOLI);
		}
		// Aggiungo la carta alla lista da restituire
		ostacoliList = ostacoloInTesta(newOstacolo, ostacoliList);
	}
	if (DBG) printf("\nDBG: read %d carteOstacolo", n);
	return ostacoliList;
}

// ========== LOG PARTITA ==============================================================================================
/**
 * logPrintLn() è la subroutine per scrivere sul file di Log
 * @param fLog FILE *: puntatore al file su cui scrivere
 * @param numTurno int: numero del turno
 * @param user string [31 + 1]: nome utente del giocatore + '\0'
 * @param carta string [31 + 1]: nome della carta
 */
void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta) {
	fprintf(fLog, "TURNO %d: %s gioca %s\n", numTurno, user, carta);
}

