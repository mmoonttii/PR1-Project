//
// Created by Monti on 24/02/24.
//

#ifndef PR1_PROJECT_MAZZOCFU_H
#define PR1_PROJECT_MAZZOCFU_H


#include "errors.h"
#include "general.h"

CartaCfu *creaMazzoCfu(FILE *fp);
CartaCfu *mescolaMazzo(CartaCfu **mazzoDaMischiare);

int contaCarte(CartaCfu *mazzoCarte);
CartaCfu *estraiCartaCfu(CartaCfu **mazzoCfu, CartaCfu *cartaCfu);
void cartaCfuInCoda(CartaCfu **mazzoCfu, CartaCfu *cartaCfu);
CartaCfu *indexEstraiCartaCfu(CartaCfu **mazzoCfu, int index);
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti);
void scartaCarte(CartaCfu **daScartare, CartaCfu **mazzoScarti);

// ============ OUTPUT ========================================================
void printSingleCartaCfu(CartaCfu *pCfu);

void printMano(CartaCfu *listaCarteCfu);

bool isIstantanea(CartaCfu *cartaCfu);
bool tutteIstantaneeCheck(CartaCfu *cartaCfu);
CartaCfu *findCartaCfu(CartaCfu **mazzoCfu, int index);
#endif//PR1_PROJECT_MAZZOCFU_H
