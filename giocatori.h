//
// Created by Monti on 07/02/24.
//

#ifndef PR1_PROJECT_GIOCATORI_H
#define PR1_PROJECT_GIOCATORI_H

#include "general.h"
#include "errors.h"

// ============ PERSONAGGI =====================================================
void parseCharacters(FILE      *fPersonaggi,
                     Character personaggi[]);
void printBonusMalus(int arr[N_OSTACOLI]);

// ============ IO ============================================================
int acquisisciNumGiocatori();
void printGiocatori(Player *listaGiocatori,
                    bool   stampaCarte);

void printGiocatore(Player *pPlayer);
// ============ LIST MANAGEMENT ===============================================
Player *addPlayerInTesta(Player *playersList, Player *newPlayer);
Player *initGiocatori(int       nGiocatori,
                      CartaCfu  **mazzoCfu,
                      Character personaggi[],
                      CartaCfu  **mazzoScarti);

Player *addPlayerInCoda(Player *playerList,
                        Player *playerToAdd);

// ============ ANDAMENTO PARTITA =============================================
bool hasLost(Player *pPlayer);
bool hasWon(Player *pPlayer);

bool playerCheck(Player        **playersList,
				 CartaOstacolo **mazzoOstacoli,
				 CartaCfu      **mazzoScarti,
				 int           *nPlayers);

#endif //PR1_PROJECT_GIOCATORI_H
