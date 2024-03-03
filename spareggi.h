//
// Created by Monti on 27/02/24.
//

#ifndef PR1_PROJECT_SPAREGGI_H
#define PR1_PROJECT_SPAREGGI_H

#include "general.h"

Player *gestisciSpareggi(int countLosers, Turno *turno, CartaCfu **mazzoScarti, CartaCfu **mazzoCfu, FILE *fLog);

void giocaCartaSpareggio(Turno *turno, CartaCfu **manoGiocatore);
#endif //PR1_PROJECT_SPAREGGI_H
