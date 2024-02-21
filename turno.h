#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - FASE 1 =========================================================================================
int acquisisciAzione();
void giocaCarta(CartaCfu **manoCarteCfu, CartaCfu **listaCarteGiocate, CartaCfu *mazzoScarti, CartaCfu *mazzoCarteCfu);
void infoGiocatori(Player *listaGiocatori, Player *currentPlayer, int nPlayers);

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
void calcolaPunteggio(Turno *turno, Player *playerList, int nPlayers);
void printPuntiParziali(Turno *turno, Player *playerList, int nPlayers);
void winnersLosers(Turno *turno, Player *playersList, int nPlayers);
// ============ CHIUSURA ===============================================================================================
void end(CartaCfu *mazzoCfu, CartaOstacolo *mazzoOstacoli, Player *playerList, CartaCfu *mazzoScarti);
#endif //PR1_PROJECT_TURNO_H
