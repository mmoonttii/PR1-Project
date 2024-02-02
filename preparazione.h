#ifndef PR1_PROJECT_PREPARAZIONE_H
#define PR1_PROJECT_PREPARAZIONE_H
#include "general.h"

// ============ UTILITIES =================================================
int randRange(int min, int max);

// ============ PERSONAGGI ================================================
FILE *openCharacters(char filePath[]);
void parseCharacters (FILE *fp, Personaggio characters[]);

// ============ CARTE CFU =================================================
FILE *openCfu(char filePath[]);
CartaCfu *nuovaCfu();
CartaCfu *mazzoCfu (FILE *fp);

// ============ CARTE OSTACOLO ================================================
FILE *openOstacoli(char *filePath);
CartaOstacolo *nuovoOstacolo();
CartaOstacolo *mazzoOstacoli(FILE *fp);

#endif //PR1_PROJECT_PREPARAZIONE_H
