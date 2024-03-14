#ifndef PR1_PROJECT_MAZZOCFU_H
#define PR1_PROJECT_MAZZOCFU_H

#include "errors.h"
#include "general.h"

// ========== LIST MANAGEMENT ==========================================================================================
CartaCfu *creaMazzoCfu(FILE *fCfu);

CartaCfu *mescolaMazzo(CartaCfu **mazzoDaMischiare);

CartaCfu *cartaCfuInTesta(CartaCfu *mazzoCfu, CartaCfu *cartaCfu);

CartaCfu *findCartaCfu(CartaCfu *mazzoCfu, int index);

CartaCfu *indexEstraiCartaCfu(CartaCfu **mazzoCfu, int index);

CartaCfu *estraiCartaCfu(CartaCfu **mazzoCfu, CartaCfu *cartaCfu);

void cartaCfuInCoda(CartaCfu **mazzoCfu, CartaCfu *cartaCfu);

CartaCfu *estraiTesta(CartaCfu **mazzoCfu);

CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti);

CartaCfu * scartaCarte(CartaCfu **daScartare, CartaCfu *mazzoScarti);

// ============ OUTPUT ========================================================
void printSingleCartaCfu(CartaCfu *pCfu);

void printMano(CartaCfu *listaCarteCfu);

// ========== VARIE ====================================================================================================
int contaCarteCfu(CartaCfu *mazzoCfu);

bool isIstantanea(CartaCfu *cartaCfu);

bool tutteIstantaneeCheck(CartaCfu *cartaCfu);

#endif//PR1_PROJECT_MAZZOCFU_H
