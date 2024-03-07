//
// Created by Monti on 07/03/24.
//

#ifndef PR1_PROJECT_ISTANTANEE_H
#define PR1_PROJECT_ISTANTANEE_H
#include "general.h"

#define ADDING 2
#define ON 2
#define OFF 1

void gestioneInstantPunteggio(int nPlayers, Player *playerList, FILE *fLog, Turno *turno, bool checkDOPPIOE);
CartaCfu *giocaInstantPunteggio(Player *pPlayer, FILE *fLog, int numTurno);
bool isIstantaneaPunteggio(CartaCfu *cartaCfu);
void risolviInstantPunteggio(CartaCfu *cartaCfu, int nPlayers, Player *playerList, Turno *turno, bool checkDOPPIOE);

void effettoAUMENTA(int nPlayers, Player *playerList, Turno *turno, bool checkDOPPIOE);
void effettoDIMINUISCI(int nPlayers, Player *playerList, Turno *turno, bool checkDOPPIOE);
void effettoINVERTI(int nPlayers, Turno *turno);

void gestioneInstantPerdita(int numTurno, Player *pPlayer, Player *playerList, CartaCfu *pCartaCfu,
                            CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo, FILE *fLog);
CartaCfu *giocaInstantPerdita(int numTurno, Player *pPlayer, FILE *fLog);
bool isIstantaneaPerdita(CartaCfu *cartaCfu);
void risolviInstantPerdita(Player *playerList, CartaCfu *pCartaCfu, CartaOstacolo **mazzoOstacoli,
                           CartaOstacolo **pCartaOstacolo);
void effettoSALVA(CartaCfu *pCartaCfu,
                  CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo);
void effettoDIROTTA(Player *playerList, CartaCfu *pCartaCfu, CartaOstacolo **pCartaOstacolo);

#endif //PR1_PROJECT_ISTANTANEE_H
