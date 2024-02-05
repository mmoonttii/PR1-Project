#ifndef PR1_PROJECT_PREPARAZIONE_H
#define PR1_PROJECT_PREPARAZIONE_H
#include "general.h"

// ============ UTILITIES =================================================
int randRange(int min, int max);
FILE *openFile(char filePath[], char mode[]);

// ============ PERSONAGGI ================================================
void parseCharacters (FILE *fp, Personaggio personaggi[]);

// ============ CARTE CFU =================================================
CartaCfu *nuovaCfu();
CartaCfu *creaMazzoCfu(FILE *fp);

CartaCfu *freeCfu(CartaCfu *mazzoCfu);
// ============ CARTE OSTACOLO ============================================
CartaOstacolo *nuovoOstacolo();
CartaOstacolo *creaMazzoOstacoli(FILE *fp);

CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli);
// ============ GIOCATORI =================================================
int acquisisciNumGiocatori();

Giocatore *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu);

#endif //PR1_PROJECT_PREPARAZIONE_H
