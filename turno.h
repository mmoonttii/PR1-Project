#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - AZIONI GIOCATORI ===============================================================================
CartaCfu *chooseCarta(CartaCfu **manoCarteCfu,
					  CartaCfu **mazzoCarteCfu, CartaCfu **mazzoScarti,
					  bool rimescolaMano);

int acquisisciCarta(int i);

void
giocaCarta(Turno *turno, Player *pPlayer, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti, FILE *fLog, bool spareggioFlag);

CartaCfu *scegliCarta(CartaCfu **manoCarteCfu);

void infoGiocatori(Player *listaGiocatori,
                   Player *currentPlayer,
                   int    nPlayers);

// ============ TURNO - CALCOLO PUNTEGGIO ==============================================================================
void calcolaPunteggio(Turno  *turno,
                      Player *playerList,
                      int    nPlayers,
					  bool   characters);

void printPunti(Turno  *turno,
                Player *playerList,
                int    nPlayers);

void winnersLosers(Turno  *turno,
                   Player *playersList,
                   int    nPlayers);

void printWinners(Player *playerList);

void printLosers(Player *playerList);

// ============ SPAREGGI ===============================================================================================
Player *gestisciSpareggi(int countLosers, Turno *turno, CartaCfu **mazzoScarti, CartaCfu **mazzoCfu, FILE *fLog);

// ============ VINCITORI ==============================================================================================
void assegnaPunti(Turno  *turno,
                  Player *playerList,
                  int    nPlayers);

// ============ PERDENTI ===============================================================================================
int contaLosers(Turno *turno);

void ostacoloInCoda(CartaOstacolo *pOstacolo,
                    CartaOstacolo **listaOstacoli);

void puntiCarteOstacolo(Player *playerList);

// ============ CHIUSURA ===============================================================================================
void end(Player *playerList, CartaCfu *mazzoCfu, CartaCfu *mazzoScarti, CartaOstacolo *mazzoOstacoli, Turno *turno);

void freeTurno(Turno *turno);

#endif //PR1_PROJECT_TURNO_H
