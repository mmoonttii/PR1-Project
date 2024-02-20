//
// Created by Monti on 07/02/24.
//

#ifndef PR1_PROJECT_CARTECFU_H
#define PR1_PROJECT_CARTECFU_H

#include "general.h"
#include "errors.h"

// ============ MEMORY MANAGEMENT =============================================
CartaCfu *allocaCartaCfu();
CartaCfu *freeCfu(CartaCfu *mazzoCfu);

// ============ LIST MANAGEMENT ===============================================
CartaCfu *creaMazzoCfu(FILE *fp);
CartaCfu *mescolaMazzo(CartaCfu **mazzoScarti);
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti);

void scartaCarte(CartaCfu **manoCarteCfu, CartaCfu *mazzoScarti);

// ============ OUTPUT ========================================================
void printSingleCartaCfu(CartaCfu *pCfu);
void printCarteCfu(CartaCfu *listaCarteCfu);

#endif //PR1_PROJECT_CARTECFU_H
