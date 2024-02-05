#ifndef PR1_PROJECT_MEMMANAGEMENT_H
#define PR1_PROJECT_MEMMANAGEMENT_H

#include "general.h"

// ============ CARTE CFU =====================================================
CartaCfu *allocaCartaCfu();
CartaCfu *freeCfu(CartaCfu *mazzoCfu);

// ============ CARTE OSTACOLO ================================================
CartaOstacolo *allocaCartaOstacolo();
CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli);

// ============ GIOCATORI =====================================================
Giocatore *allocaGiocatore();
Giocatore *freeGiocatore(Giocatore *listaGiocatore);

// ============ FREE ALL ======================================================
void freeAll(FILE *fPersonaggi, FILE *fOstacoli, FILE *fCfu,
			 CartaCfu *mazzoCfu, CartaOstacolo *mazzoOstacoli,Giocatore *listaGiocatori);

#endif //PR1_PROJECT_MEMMANAGEMENT_H
