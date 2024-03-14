#ifndef PR1_PROJECT_GIOCATORI_H
#define PR1_PROJECT_GIOCATORI_H

#include "general.h"
#include "errors.h"

// ============ PERSONAGGI =====================================================
void parseCharacters(FILE      *fPersonaggi,
                     Character personaggi[]);

void printBonusMalus(int arr[N_OSTACOLI]);

void printCharacter(Character *pCharacter);
// ============ IO ============================================================
int acquisisciNumGiocatori();

void printGiocatore(Player *pPlayer);

void printGiocatori(Player *listaGiocatori);
// ============ LIST MANAGEMENT ===============================================
Player *addPlayerInTesta(Player *playersList, Player *newPlayer);

Player *initGiocatori(int nGiocatori, Character personaggi[],
					  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti);

bool checkUsernames(Player *playersList, char *username);

Player *addCopyOfPlayerInCoda(Player *playerList,
                              Player *playerToAdd);

// ============ ANDAMENTO PARTITA =============================================
bool hasLost(Player *pPlayer);

bool hasWon(Player *pPlayer);

bool playerCheck(int *nPlayers, Player **playersList, CartaOstacolo **mazzoOstacoli, CartaCfu **mazzoScarti);

#endif //PR1_PROJECT_GIOCATORI_H
