#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - AZIONI GIOCATORI ===============================================================================
int acquisisciAzione();

void giocaCarta(CartaCfu **manoCarteCfu,
                CartaCfu **listaCarteGiocate,
                CartaCfu *mazzoScarti,
                CartaCfu *mazzoCarteCfu);

void infoGiocatori(Player *listaGiocatori,
                   Player *currentPlayer,
                   int    nPlayers);

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
void calcolaPunteggio(Turno  *turno,
                      Player *playerList,
                      int    nPlayers);

void printPuntiParziali(Turno  *turno,
                        Player *playerList,
                        int    nPlayers);

void winnersLosers(Turno  *turno,
                   Player *playersList,
                   int    nPlayers);

void printWinners(Player *playerList);

void printLosers(Player *playerList);

// ============ VINCITORI ==============================================================================================
void assegnaPunti(Turno  *turno,
                  Player *playerList,
                  int    nPlayers);

// ============ PERDENTI ===============================================================================================
void spareggiCheck(Turno  *turno,
                   Player *playerList);

void ostacoloInCoda(CartaOstacolo **pOstacolo,
                    CartaOstacolo *listaOstacoli);

void puntiCarteOstacolo(Player *playerList);

// ============ CHIUSURA ===============================================================================================
void loseCondition(Player *playersList);

bool winConditions(Player *playersList);

void end(CartaCfu      *mazzoCfu,
         CartaOstacolo *mazzoOstacoli,
         Player        *playerList,
         CartaCfu      *mazzoScarti);
#endif //PR1_PROJECT_TURNO_H
