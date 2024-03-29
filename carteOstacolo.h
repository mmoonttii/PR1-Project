#ifndef PR1_PROJECT_CARTEOSTACOLO_H
#define PR1_PROJECT_CARTEOSTACOLO_H

#include "general.h"
#include "errors.h"

// ============ LIST MANAGEMENT ===============================================/**
CartaOstacolo *creaMazzoOstacoli(FILE *fOstacoli);

CartaOstacolo *ostacoloInTesta(CartaOstacolo *mazzoOstacoli, CartaOstacolo *cartaOstacolo);

CartaOstacolo *mescolaMazzoOstacoli(CartaOstacolo **mazzoDaMischiare);

CartaOstacolo *indexEstraiCartaOstacolo(CartaOstacolo **mazzoOstacoli, int index);

CartaOstacolo *pescaCartaOstacolo(CartaOstacolo **mazzoOstacoli);

void ostacoloInCoda(CartaOstacolo *pOstacolo, CartaOstacolo **listaOstacoli);

// ============ OUTPUT ========================================================
void printOstacoli(CartaOstacolo *listaOstacoli);

int contaOstacoli(CartaOstacolo *listaOstacoli);

#endif //PR1_PROJECT_CARTEOSTACOLO_H
