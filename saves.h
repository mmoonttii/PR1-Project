#ifndef PR1_PROJECT_SAVES_H
#define PR1_PROJECT_SAVES_H

#include "general.h"

#define SAVES_UNIT 1

// ========== SALVATAGGIO ==============================================================================================
void saveOnFile(char *saveName, FILE *fSave,
				int *nPlayers, Player *playersList,
				CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
				CartaOstacolo *mazzoOstacoli);

void writePlayersList(FILE *fSave, int n, Player *playersList);

void writeCfuList(FILE *fSave, int n, CartaCfu *cfuList);

void writeOstacoliList(FILE *fSave, int n, CartaOstacolo *ostacoliList);

// ========== CARICAMENTO ==============================================================================================
void loadSaveFromFile(char *saveName, FILE *fSave,
					  int *nPlayers, Player **playerList,
					  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
					  CartaOstacolo **mazzoOstacoli);

Player *readPlayersList(FILE *fSave, int nPlayers);

CartaCfu *readCfuList(FILE *fSave, int n);

CartaOstacolo *readOstacoliList(FILE *fSave, int n);

// ========== LOG PARTITA ==============================================================================================
void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta);


#endif //PR1_PROJECT_SAVES_H
