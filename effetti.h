//
// Created by Monti on 26/02/24.
//

#ifndef PR1_PROJECT_EFFETTI_H
#define PR1_PROJECT_EFFETTI_H
#include "general.h"
#include "mazzoCfu.h"
#include "turno.h"
#include "memoria.h"

void gestioneEffetti(Turno *turno, Player *playerList, int nPlayers, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti);

bool risolviEffetti(Player *playerList, CartaCfu *pCarta, Player *pPlayer, int nPlayers);

void effettoSCARTAP();

#endif //PR1_PROJECT_EFFETTI_H
