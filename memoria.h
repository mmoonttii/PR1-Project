//
// Created by Monti on 24/02/24.
//

#ifndef PR1_PROJECT_MEMORIA_H
#define PR1_PROJECT_MEMORIA_H

#include "errors.h"
#include "general.h"
// ============ CARTE CFU =============================================

CartaCfu *allocaCartaCfu();
CartaCfu *freeCfu(CartaCfu *mazzoCfu);

// ============ CARTE OSTACOLO =============================================
CartaOstacolo *allocaCartaOstacolo();
CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli);

// ========== GIOCATORI ========================================================
Player *allocaGiocatore();
Player *freeGiocatore(Player *listaGiocatore);

int *freeIntArr(int *arr);

bool *allocaArrBool(int n);
bool *freeBoolArr(bool *arr);

void freeAll(CartaCfu *mazzoCfu, CartaCfu *mazzoScarti,
			 CartaOstacolo *mazzoOstacoli,
			 Player *playerList);
#endif//PR1_PROJECT_MEMORIA_H
