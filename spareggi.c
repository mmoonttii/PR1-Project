//
// Created by Monti on 27/02/24.
//

#include "spareggi.h"
#include "mazzoCfu.h"
#include "turno.h"

Player *gestisciSpareggi(int countLosers, Turno *turno, CartaCfu **mazzoScarti, CartaCfu **mazzoCfu, FILE *fLog) {
	Turno spareggio = {};
	Player *playerList = NULL,
		   *playerHead = NULL,
		   *pLoser = NULL;
	CartaCfu *headMano = NULL;
	bool tutteIstantanee;

	playerList = turno->losers,
	playerHead = playerList;

	for (int i = 0; i < countLosers; ++i) {
		headMano = playerHead->manoCarteCfu;
		// infoPlayer
		if (contaCarte(headMano) > 0 && !tutteIstantaneeCheck(headMano)) {
			giocaCarta(&spareggio, playerHead, mazzoCfu, mazzoCfu, fLog, SPAREGGIO);
		} else {
			addPlayerInCoda(spareggio.losers, playerHead);
		}
		if (playerHead->nextPlayer != NULL) {
			playerHead = playerHead->nextPlayer;
		}
	}

	// ==== CALCOLO PUNTEGGI ==========
	calcolaPunteggio(&spareggio, playerList, countLosers, SPAREGGIO);
	printPuntiParziali(&spareggio, playerList, countLosers);

	winnersLosers(&spareggio, playerList, countLosers);
	printLosers(spareggio.losers);
	printWinners(spareggio.winners);


	countLosers = contaLosers(&spareggio, playerList); // Conta i giocatori che hanno perso

	// Check read brutti
	if (countLosers == 1) {
		playerHead = playerList;
		while (spareggio.losers != NULL) {
			if (strcmp(spareggio.losers->username, playerHead->username) == 0) {
				pLoser = playerHead;
			}
			spareggio.losers = spareggio.losers->nextPlayer;
		}
	} else {
		printf("\nRisoluzione spareggi");
		pLoser = gestisciSpareggi(countLosers, &spareggio, mazzoScarti, mazzoCfu, fLog);
	}

	return pLoser;
}