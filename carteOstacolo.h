//	getchar();
// Created by Monti on 07/02/24.
//

#ifndef PR1_PROJECT_CARTEOSTACOLO_H
#define PR1_PROJECT_CARTEOSTACOLO_H

#include "general.h"
#include "errors.h"

// ============ LIST MANAGEMENT ===============================================/**
CartaOstacolo *creaMazzoOstacoli(FILE *fp);
CartaOstacolo *pescaCartaOstacolo(CartaOstacolo **mazzoOstacoli);
void ostacoloInCoda(CartaOstacolo **pOstacolo, CartaOstacolo **listaOstacoli);
// ============ OUTPUT ========================================================
void printOstacoli(CartaOstacolo *listaOstacoli);

#endif //PR1_PROJECT_CARTEOSTACOLO_H