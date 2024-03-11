//
// Created by Monti on 26/02/24.
//

#ifndef PR1_PROJECT_EFFETTI_H
#define PR1_PROJECT_EFFETTI_H

#include "general.h"
#include "mazzoCfu.h"
#include "turno.h"
#include "memoria.h"

#define MAX_SCARTABILI 3
#define CARTE_SBIRCIABILI 2

void gestioneEffetti(int nPlayers, Player *playerList, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti, Turno *turno,
                     bool *checkDOPPIOE);

bool
risolviEffetti(int iPlayer, Player *currPlayer, int nPlayers, Player *playerList, CartaCfu *currCarta, CartaCfu **mazzoCfu,
               CartaCfu **mazzoScarti, Turno *turno, bool arrRisolte[], bool *checkDOPPIOE);

// ===== EFFETTI DELLE CARTE ====================
void effettoSCARTAP(CartaCfu **mazzoScarti, Player *pPlayer, int iPlayer, Turno *turno);

// ====================================================================================================================
void effettoRUBA(Player *playerList, Player *pPlayer);
CartaCfu *steal(Player *pPlayer);

// ====================================================================================================================
void effettoSCAMBIADS(int iPlayer, Player *pPlayer, int nPlayers, Player *playerList, CartaCfu *pCarta, Turno *turno,
                      bool arrRisolte[]);
// ====================================================================================================================
void effettoSCARTAE(int iPlayer, Player *pPlayer, CartaCfu **mazzoScarti, Turno *turno);
CartaCfu *discard(Player *pPlayer);

// ====================================================================================================================
void effettoSCARTAC(Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAP(Turno *turno, int nPlayers, Player *playerList);

// ====================================================================================================================
void effettoDOPPIOE(bool *checkDOPPIOE);

// ====================================================================================================================
void effettoSBIRCIA(CartaCfu **mazzoCfu, Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAC(CartaCfu **carteGiocate, Player *playerList, bool arrRisolte[]);

#endif //PR1_PROJECT_EFFETTI_H
