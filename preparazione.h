#ifndef PR1_PROJECT_PREPARAZIONE_H
#define PR1_PROJECT_PREPARAZIONE_H
#include "general.h"

// ============ PERSONAGGI ================================================
void parseCharacters (FILE *fp, Personaggio characters[]);

// ============ CARTE CFU =================================================
CartaCfu *nuovaCfu();

CartaCfu *mazzoCfu (FILE *fp);

#endif //PR1_PROJECT_PREPARAZIONE_H
