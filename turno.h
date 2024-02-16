#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - FASE 1 ================================================
int acquisisciAzione();

void giocaCarta(CartaCfu *manoCarteCfu, CartaCfu *listaCarteGiocate);

void infoGiocatori(Player *listaGiocatori, Player *currentPlayer);

#endif //PR1_PROJECT_TURNO_H
