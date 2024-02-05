#ifndef PR1_PROJECT_PREPARAZIONE_H
#define PR1_PROJECT_PREPARAZIONE_H
#include "general.h"
#include "memManagement.h"

// ============ UTILITIES =================================================
int randRange(int min, int max);
FILE *openFile(char filePath[], char mode[]);

// ============ PERSONAGGI ================================================
void parseCharacters (FILE *fp, Personaggio personaggi[]);

// ============ CARTE CFU =================================================
CartaCfu *creaMazzoCfu(FILE *fp);
CartaCfu *distribuisciCarte(CartaCfu **mazzoCfu);

// ============ CARTE OSTACOLO ============================================
CartaOstacolo *creaMazzoOstacoli(FILE *fp);

// ============ GIOCATORI =================================================
int acquisisciNumGiocatori();

Giocatore *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu);

#endif //PR1_PROJECT_PREPARAZIONE_H
