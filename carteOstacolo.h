//
// Created by Monti on 07/02/24.
//

#ifndef PR1_PROJECT_CARTEOSTACOLO_H
#define PR1_PROJECT_CARTEOSTACOLO_H

#include "general.h"
#include "errors.h"
// ============ MEMORY MANAGEMENT =============================================
CartaOstacolo *allocaCartaOstacolo();
CartaOstacolo *freeOstacoli(CartaOstacolo *mazzoOstacoli);

// ============ LIST MANAGEMENT ===============================================/**

CartaOstacolo *creaMazzoOstacoli(FILE *fp);
CartaOstacolo *pescaCartaOstacolo(CartaOstacolo **mazzoOstacoli);

// ============ OUTPUT ========================================================
void printOstacoli(CartaOstacolo *listaOstacoli);

#endif //PR1_PROJECT_CARTEOSTACOLO_H
