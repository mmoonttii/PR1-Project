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

void gestioneEffetti(int nPlayers, Player *playerList,
					 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
					 Turno *turno);

bool
risolviEffetti(int iPlayer, Player *pPlayer,
			   int nPlayers, Player *playerList,
			   CartaCfu *pCarta, CartaCfu **mazzoCfu,
               CartaCfu **mazzoScarti,
			   Turno *turno, bool arrRisolte[]);

// ===== EFFETTI DELLE CARTE ====================
void effettoSCARTAP(CartaCfu **mazzoScarti, Player *pPlayer, Turno *turno, int iPlayer);

// ====================================================================================================================
void effettoRUBA(Player *playerList, Player *pPlayer);
CartaCfu *steal(Player *pPlayer);

// ====================================================================================================================
void effettoSCAMBIADS(Player *playerList, Player *pPlayer, Turno *turno, int iPlayer, int nPlayers, CartaCfu *pCarta,
                      bool arrRisolte[]);
// ====================================================================================================================
void effettoSCARTAE(CartaCfu **mazzoScarti, Player *pPlayer, Turno *turno, int iPlayer);
CartaCfu *discard(Player *pPlayer);

// ====================================================================================================================
void effettoSCARTAC(Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAP(Turno *turno, int nPlayers, Player *playerList);

// ====================================================================================================================
void effettoDOPPIOE();

// ====================================================================================================================
void effettoSBIRCIA(CartaCfu **mazzoCfu, Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAC(CartaCfu **carteGiocate, Player *playerList, bool arrRisolte[]);

#endif //PR1_PROJECT_EFFETTI_H
