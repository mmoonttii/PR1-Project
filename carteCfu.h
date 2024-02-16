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
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu);

// ============ OUTPUT ========================================================
int printCarteCfu(CartaCfu *mazzoCfu);
int choiceCarta(int count);

#endif //PR1_PROJECT_CARTECFU_H
