//
// Created by Monti on 28/02/24.
//

#include "saves.h"
#include "memoria.h"
#include "mazzoCfu.h"
#include "carteOstacolo.h"

void saveOnFile(char *saveName, FILE *fSave,
                Character charactersArr[], int *nPlayers, Player *playerList,
                CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
                CartaOstacolo *mazzoOstacoli){

	int nOstacoli = 0,
		nCfu      = 0;
	Player *headPlayers = NULL;
	CartaOstacolo *headOstacoli = NULL;
	CartaCfu *headCfu = NULL;

	rewind(fSave);
	printf("Salvataggio su %s in corso...", saveName);

	headPlayers = playerList;

	// Scrittura numero Giocatori
	fwrite(nPlayers, sizeof(int), 1, fSave);
	// Scrittura Giocatori
	for (int i = 0; i < *nPlayers; ++i) {
		fwrite(headPlayers, sizeof(Player), 1, fSave);

		// Scrittura carte Cfu Mano
		headCfu = headPlayers->manoCarteCfu;
		for (int j = 0; j < CARTE_PER_MANO; ++j) {
			fwrite(headCfu, sizeof(CartaCfu), 1, fSave);
			if (headCfu->next != NULL) {
				headCfu = headCfu->next;
			}
		}

		// Scrittura numero carte Ostacolo
		headOstacoli = headPlayers->listaCarteOstacolo;
		nOstacoli = contaOstacoli(headOstacoli);
		fwrite(&nOstacoli, sizeof(int), 1, fSave);

		// Scrittura carte Ostacolo
		for (int j = 0; j < nOstacoli; ++j) {
			fwrite(headOstacoli, sizeof(CartaOstacolo), 1, fSave);
			if (headOstacoli->next != NULL) {
				headOstacoli = headOstacoli->next;
			}
		}
		headPlayers = headPlayers->nextPlayer;
	}

	// Scrittura numero carte Cfu mazzo
	headCfu = mazzoCfu;
	nCfu = contaCarteCfu(headCfu);
	fwrite(&nCfu, sizeof(int), 1, fSave);

	// Scrittura carte Cfu mazzo
	for (int i = 0; i < nCfu; i++) {
		fwrite(headCfu, sizeof(CartaCfu), 1, fSave);
		if (headCfu->next != NULL) {
			headCfu = headCfu->next;
		}
	}

	// Scrittura numero carte Cfu scarti
	headCfu = mazzoScarti;
	nCfu = contaCarteCfu(headCfu);
	fwrite(&nCfu, sizeof(int), 1, fSave);

	// Scrittura carte Cfu scarti
	for (int i = 0; i < nCfu; i++) {
		fwrite(headCfu, sizeof(CartaCfu), 1, fSave);
		if (headCfu->next != NULL) {
			headCfu = headCfu->next;
		}
	}

	// Scrittura numero carte Ostacolo mazzo
	headOstacoli = mazzoOstacoli;
	nOstacoli = contaOstacoli(headOstacoli);
	fwrite(&nOstacoli, sizeof(int), 1, fSave);

	// Scrittura carte Ostacolo mazzo
	for (int j = 0; j < nOstacoli; ++j) {
		fwrite(headOstacoli, sizeof(CartaOstacolo), 1, fSave);
		if (headOstacoli->next != NULL) {
			headOstacoli = headOstacoli->next;
		}
	}
}

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
	check = fread(nPlayers, sizeof(int), 1, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_PLAYERS);
	}

	// Lettura Players
	for (int i = 0; i < *nPlayers; ++i) {
		check = 0;
		check = fread(&playerAux, sizeof(Player), 1, fSave);
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
			check = fread(&carteAux, sizeof(CartaCfu), 1, fSave);
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
		check            = fread(&nManoOstacoli, sizeof(int), 1, fSave);
		if (check == 0) {
			exit(ERR_READ_SAVE_N_OSTACOLI);
		}
		for (int j = 0; j < nManoOstacoli; ++j) {
			check = 0;
			check = fread(&ostacoloAux, sizeof(CartaOstacolo), 1, fSave);
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
	check = fread(&nCfuMazzo, sizeof(int), 1, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_CFU);
	}

	// Lettura carte Cfu
	for (int i = 0; i < nCfuMazzo; ++i) {
		check = 0;
		check = fread(&carteAux, sizeof(CartaCfu), 1, fSave);
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
	check = fread(&nCfuScarti, sizeof(int), 1, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_SCARTI);
	}

	// Lettura carte Cfu scrti
	for (int i = 0; i < nCfuScarti; ++i) {
		check = 0;
		check = fread(&carteAux, sizeof(CartaCfu), 1, fSave);
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
	check = fread(&nMazzoOstacoli, sizeof(int), 1, fSave);
	if (check == 0) {
		exit(ERR_READ_SAVE_N_MAZZO_OSTACOLI);
	}
	// Lettura carte Ostacolo mazzo
	for (int j = 0; j < nManoOstacoli; ++j) {
		check = 0;
		check = fread(&ostacoloAux, sizeof(CartaOstacolo), 1, fSave);
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

