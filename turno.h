#ifndef PR1_PROJECT_TURNO_H
#define PR1_PROJECT_TURNO_H

#include "general.h"
#include "errors.h"
#include "giocatori.h"

// ============ TURNO - GIOCA CARTA ====================================================================================
CartaCfu *chooseCarta(CartaCfu **manoCarteCfu,
					  CartaCfu **mazzoCarteCfu, CartaCfu **mazzoScarti,
					  bool rimescolaMano);

void giocaCarta(Turno *turno, Player *pPlayer,
				CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
				FILE *fLog, bool spareggioFlag);

// ============ TURNO - INFO GIOCATORE =================================================================================
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

// ============ SPAREGGI ===============================================================================================
Player *gestisciSpareggi(int countLosers, Turno *turno, CartaCfu **mazzoScarti, CartaCfu **mazzoCfu, FILE *fLog);

// ============ VINCITORI ==============================================================================================
void printWinners(Player *playerList);

void assegnaPunti(Turno  *turno,
                  Player *playerList,
                  int    nPlayers);

// ============ PERDENTI ===============================================================================================
void printLosers(Player *playerList);

int contaLosers(Turno *turno);

void puntiCarteOstacolo(Player *playerList);

void ostacoloALoser(Turno *turno, Player *playersList, Player *pLoser);

// ============ CHIUSURA ===============================================================================================
void end(Player *playerList, CartaCfu *mazzoCfu, CartaCfu *mazzoScarti, CartaOstacolo *mazzoOstacoli, Turno *turno);

void freeTurno(Turno *turno);

#endif //PR1_PROJECT_TURNO_H
