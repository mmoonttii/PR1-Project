#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - AZIONI GIOCATORI ===============================================================================
int acquisisciAzione();

CartaCfu *chooseCarta(CartaCfu **manoCarteCfu, CartaCfu **mazzoScarti, CartaCfu **mazzoCarteCfu, bool rimescolaMano);

int acquisisciCarta(int i);

void giocaCarta(Turno    *turno,
                Player   *pPlayer,
                CartaCfu **mazzoScarti,
                CartaCfu **mazzoCfu,
                FILE     *fLog,
                bool     spareggioFlag);

CartaCfu *scegliCarta(CartaCfu **manoCarteCfu);

void infoGiocatori(Player *listaGiocatori,
                   Player *currentPlayer,
                   int    nPlayers);

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
void calcolaPunteggio(Turno  *turno,
                      Player *playerList,
                      int    nPlayers,
					  bool   characters);

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
int contaLosers(Turno  *turno,
                Player *playerList);

void ostacoloInCoda(CartaOstacolo **pOstacolo,
                    CartaOstacolo **listaOstacoli);

void puntiCarteOstacolo(Player *playerList);

// ============ CHIUSURA ===============================================================================================

void end(CartaCfu      *mazzoCfu,
         CartaOstacolo *mazzoOstacoli,
         Player        *playerList,
         CartaCfu      *mazzoScarti);

#endif //PR1_PROJECT_TURNO_H
