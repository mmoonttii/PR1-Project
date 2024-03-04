//
// Created by Monti on 28/02/24.
//

#include "saves.h"

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
	printf("Lettura da %s in corso...", saveName);

}

void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta) {
	fprintf(fLog, "TURNO %d: %s gioca %s\n", numTurno, user, carta);
}

