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
					 Turno *turno, bool *checkDOPPIOE);

bool risolviEffetti(int iPlayer, Player *currPlayer,
			   int nPlayers, Player *playerList,
			   CartaCfu *currCarta,
			   CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			   Turno *turno,
			   bool arrRisolte[], bool *checkDOPPIOE);

// ===== EFFETTI DELLE CARTE ====================
void effettoSCARTAP(int iPlayer, Player *pPlayer,
					CartaCfu **mazzoScarti, Turno *turno);

// ====================================================================================================================
void effettoRUBA(Player *playerList, Player *pPlayer);

CartaCfu *steal(Player *pPlayer);

// ====================================================================================================================
void effettoSCAMBIADS(int iPlayer, Player *pPlayer,
				 int nPlayers, Player *playerList,
				 CartaCfu *cartaPlayer,
				 Turno *turno, bool arrRisolte[]);

// ====================================================================================================================
void effettoSCARTAE(int iPlayer, Player *pPlayer,
					CartaCfu **mazzoScarti, Turno *turno);

CartaCfu *discard(Player *pPlayer);

// ====================================================================================================================
void effettoSCARTAC(Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAP(int nPlayers, Player *playerList, Turno *turno);

// ====================================================================================================================
void effettoDOPPIOE(bool *checkDOPPIOE);

// ====================================================================================================================
void effettoSBIRCIA(CartaCfu **mazzoCfu, Player *pPlayer, CartaCfu **mazzoScarti);

// ====================================================================================================================
void effettoSCAMBIAC(int nPlayers, Player *playerList,
					 CartaCfu **carteGiocate, bool arrRisolte[]);

int chooseSCAMBIAC(Player *playerList, CartaCfu *carte, int nPlayers);

#endif //PR1_PROJECT_EFFETTI_H
