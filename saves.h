//
// Created by Monti on 28/02/24.
//

#ifndef PR1_PROJECT_SAVES_H
#define PR1_PROJECT_SAVES_H

#include "general.h"

#define SAVES_UNIT 1

void saveOnFile(char *saveName, FILE *fSave,
                Character charactersArr[], int *nPlayers, Player *playersList,
                CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
                CartaOstacolo *mazzoOstacoli);

void writePlayersList(FILE *fSave, int n, Player *playersList);
void writeCfuList(FILE *fSave, int n, CartaCfu *cfuList);
void writeOstacoliList(FILE *fSave, int n, CartaOstacolo *ostacoliList);

void loadSaveFromFile(char *saveName, FILE *fSave,
                      Character charactersArr[], int *nPlayers, Player **playerList,
                      CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
                      CartaOstacolo **mazzoOstacoli);

void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta);


#endif //PR1_PROJECT_SAVES_H
