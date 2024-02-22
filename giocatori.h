//
// Created by Monti on 07/02/24.
//

#ifndef PR1_PROJECT_GIOCATORI_H
#define PR1_PROJECT_GIOCATORI_H

#include "general.h"
#include "errors.h"
#include "carteCfu.h"

// ============ PERSONAGGI =====================================================
void parseCharacters(FILE *fp, Character personaggi[]);

// ============ MEMORY MANAGEMENT =============================================
Player *allocaGiocatore();
Player *freeGiocatore(Player *listaGiocatore);

// ============ IO ============================================================
int acquisisciNumGiocatori();
void printGiocatori(Player *listaGiocatori, bool stampaCarte);
void printGiocatore(Player *giocatore, bool stampaCarte);
// ============ LIST MANAGEMENT ===============================================
Player *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu, Character personaggi[], CartaCfu *mazzoScarti);
Player *addPlayerInCoda(Player *playerList, Player *playerToAdd);
#endif //PR1_PROJECT_GIOCATORI_H
