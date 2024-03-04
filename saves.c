//
// Created by Monti on 28/02/24.
//

#include "saves.h"
#include "memoria.h"

void saveOnFile(char *saveName, FILE *fSave,
                Character charactersArr[], int *nPlayers, Player **playerList,
                CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                CartaOstacolo **mazzoOstacoli){
	rewind(fSave);
	printf("Salvataggio su %s in corso...", saveName);

}

void loadSaveFromFile(char *saveName, FILE *fSave,
                      Character charactersArr[], int *nPlayers, Player **playerList,
                      CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                      CartaOstacolo **mazzoOstacoli){
    int check = 0,
		nOstacoli = 0;
    Player playerAux,
           *playerHead = NULL;
	CartaCfu carteAux,
			 *headCarte = NULL;
	CartaOstacolo ostacoloAux,
				  *headOstacoli = NULL;

    rewind(fSave);
    printf("Lettura da %s in corso...", saveName);
    check = fread(nPlayers, sizeof(int), 1, fSave);

    if (check == 0) {
        exit(ERR_READ_SAVE_NPLAYERS);
    }

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

		playerHead->manoCarteCfu = NULL;
        for (int j = 0; j < CARTE_PER_MANO; ++j) {
	        check = 0;
			check = fread(&carteAux, sizeof(CartaCfu), 1, fSave);
	        if (check == 0) {
		        exit(ERR_READ_SAVE_CFU);
	        }
			if (playerHead->manoCarteCfu == NULL) {
				playerHead->manoCarteCfu = allocaCartaCfu();
				*(playerHead->manoCarteCfu) = carteAux;
				playerHead->manoCarteCfu = headCarte;
			} else {
				headCarte->next = allocaCartaCfu();
				*(headCarte->next) = carteAux;
				headCarte = headCarte->next;
			}
        }
		check = 0;
	    check = fread(&nOstacoli, sizeof(int), 1, fSave);
	    if (check == 0) {
		    exit(ERR_READ_SAVE_NOSTACOLI);
	    }
	    for (int j = 0; j < nOstacoli; ++j) {
			check = 0;
		    check = fread(&ostacoloAux, sizeof(CartaOstacolo), 1, fSave);
		    if (check == 0) {
			    exit(ERR_READ_SAVE_OSTACOLI);
		    }

		}
    }

}

void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta) {
	fprintf(fLog, "TURNO %d: %s gioca %s\n", numTurno, user, carta);
}

