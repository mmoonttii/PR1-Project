//
// Created by Monti on 28/02/24.
//

#include "salvataggiLog.h"

void logPrintLn(FILE *fLog, int numTurno, char *user, char *carta) {
	fprintf(fLog, "TURNO %d: %s gioca %s\n", numTurno, user, carta);
}

