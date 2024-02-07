#ifndef PR1_PROJECT_PREPARAZIONE_H
#define PR1_PROJECT_PREPARAZIONE_H
#include "general.h"
#include "memManagement.h"

// ============ UTILITIES =================================================
int randRange(int min, int max);
FILE *openFile(char filePath[], char mode[]);

// ============ CARTE CFU =================================================
CartaCfu *creaMazzoCfu(FILE *fp);
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu);

void printCarteCfu(CartaCfu *mazzoCfu);
// ============ CARTE OSTACOLO ============================================
CartaOstacolo *creaMazzoOstacoli(FILE *fp);

// ============ PERSONAGGI E GIOCATORI =================================================
void parseCharacters (FILE *fp, Personaggio personaggi[]);


int acquisisciNumGiocatori();
Giocatore *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu, Personaggio personaggi[]);
void printGiocatori(Giocatore *listaGiocatori);

#endif //PR1_PROJECT_PREPARAZIONE_H
