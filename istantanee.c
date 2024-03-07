//
// Created by Monti on 07/03/24.
//

#include "istantanee.h"
#include "mazzoCfu.h"
#include "turno.h"
#include "saves.h"


void gestioneInstantPunteggio(int nPlayers, Player *playerList, FILE *fLog, Turno *turno) {
	int choice = 0;
	Player *playerHead = NULL;
	CartaCfu *headCarte  = NULL,
			 *playedCard = NULL;
	bool stop         = false;
	playerHead = playerList;

	while (!stop) {
		printf("\nChi vuole giocare una carta istantanea?\n");
		for (int i = 0; i < nPlayers; ++i) {
			printf("[%d] %s\n", i, playerHead->username);
		}
		printf("[0] Nessuno vuole giocare carte istantanee\n");

		do {
			printf(">>> ");
			scanf("%d", &choice);

			if (choice < 0 || choice > nPlayers) {
				printf("Input non valido, riprovare\n");
			}
		} while (choice < 0 || choice > nPlayers);

		if (choice == 0) {
			stop = true;
		} else {
			choice--;
			for (int i = 0; i < choice; ++i) {
				playerHead = playerHead->nextPlayer;
			}
			headCarte = playerHead->manoCarteCfu;
			playedCard = giocaInstantPunteggio(playerHead, fLog, turno->numTurno);

			risolviInstantPunteggio(playedCard, nPlayers, playerList, turno);
		}
	}
}

CartaCfu *giocaInstantPunteggio(Player *pPlayer, FILE *fLog, int numTurno) {
	CartaCfu *headCarte        = pPlayer->manoCarteCfu,
	         *choosenCard      = NULL;
	int      i                 = 0,
	         choice            = 0;
	bool     atLeastOneInstant = false,
	         instantCheck      = false;

	while (headCarte != NULL) {
		if (isIstantaneaPunteggio(headCarte)) {
			atLeastOneInstant = true;
		}
		printf("[%d] ", i);
		printSingleCartaCfu(headCarte);
		headCarte = headCarte->next;
	}

	if (atLeastOneInstant) {
		do {
			choice    = acquisisciCarta(i);
			headCarte = pPlayer->manoCarteCfu;

			choosenCard = findCartaCfu(&headCarte, choice);
			instantCheck = isIstantaneaPunteggio(choosenCard);

			if (!instantCheck) {
				printf("\nHai selezionato una carta che non è istantanea, riprova");
			}

		} while (!instantCheck);

		choosenCard = estraiCartaCfu(&headCarte, choosenCard);
		logPrintLn(fLog, numTurno, pPlayer->username, choosenCard->name);
	} else {
		printf("\nNon hai carte istantanee in mano\n");
		choosenCard = NULL;
	}

	return choosenCard;
}

bool isIstantaneaPunteggio(CartaCfu *cartaCfu) {
	bool ret = false;
	if (cartaCfu->effect == AUMENTA || cartaCfu->effect == DIMINUISCI || cartaCfu->effect == INVERTI) {
		ret = true;
	}
	return ret;
}

void risolviInstantPunteggio(CartaCfu *cartaCfu, int nPlayers, Player *playerList, Turno *turno) {
	switch (cartaCfu->effect) {
		case AUMENTA:
			effettoAUMENTA(nPlayers, playerList, turno);
			minMax(turno->points, nPlayers, &(turno->cfuToLose), &(turno->cfuToWin));
			break;
		case DIMINUISCI:
			effettoDIMINUISCI(nPlayers, playerList, turno);
			minMax(turno->points, nPlayers, &(turno->cfuToLose), &(turno->cfuToWin));
			break;
		case INVERTI:
			effettoINVERTI(nPlayers, turno);
			break;
		default:
			printf("Errore");
			break;
	}
}

void effettoAUMENTA(int nPlayers, Player *playerList, Turno *turno) {
	int choice = 0;
	Player *playerHead = NULL;

	playerHead = playerList;
	printf("\nA chi vuoi aumentare il punteggio?\n");
	for (int i = 0; i < nPlayers; ++i) {
		printf("[%d] %s\n", i, playerHead->username);
		playerHead = playerHead->nextPlayer;
	}
	do {
		printf(">>> [0 - %d] ", nPlayers - 1);
		scanf("%d", &choice);

		if (choice < 0 || choice > nPlayers - 1) {
			printf("Input non valido, riprovare");
		}
	} while (choice < 0 || choice > nPlayers - 1);

	turno->points[choice] += 2;
}
void effettoDIMINUISCI(int nPlayers, Player *playerList, Turno *turno) {
	int choice = 0;
	Player *playerHead = NULL;

	playerHead = playerList;
	printf("\nA chi vuoi diminuire il punteggio?\n");
	for (int i = 0; i < nPlayers; ++i) {
		printf("[%d] %s\n", i, playerHead->username);
		playerHead = playerHead->nextPlayer;
	}
	do {
		printf(">>> [0 - %d] ", nPlayers - 1);
		scanf("%d", &choice);

		if (choice < 0 || choice > nPlayers - 1) {
			printf("Input non valido, riprovare");
		}
	} while (choice < 0 || choice > nPlayers - 1);

	turno->points[choice] -= 2;
}
void effettoINVERTI(int nPlayers, Turno *turno) {
	for (int i = 0; i < nPlayers; ++i) {
		if (turno->points[i] == turno->cfuToWin) {
			turno->points[i] = turno->cfuToLose;
		}
		if (turno->points[i] == turno->cfuToLose) {
			turno->points[i] = turno->cfuToWin;
		}
	}
}

void gestioneInstantPerdita(int numTurno,
							Player *pPlayer, Player *playerList,
							CartaCfu *pCartaCfu,
                            CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo,
							FILE *fLog) {
	char choice;
	CartaCfu *choosenCard = NULL;
	printf("\nHai perso il turno, vuoi controllare se puoi giocare una carta istantanea?");
	do {
		printf("\n>>> [y/n]");
		scanf(" %c", &choice);

		if (choice != 'y' || choice != 'n') {
			printf("\nInput non valido, riprovare");
		}
	} while (choice != 'y' || choice != 'n');

	switch (choice) {
		case 'y':
			choosenCard = giocaInstantPerdita(numTurno, pPlayer, fLog);
			break;
		case 'n':
			ostacoloInCoda(*pCartaOstacolo, &(pPlayer->listaCarteOstacolo));
			break;
		default:
			printf("\nErrore\n");
	}
	if (choosenCard != NULL) {
		risolviInstantPerdita(playerList, choosenCard, mazzoOstacoli, pCartaOstacolo);
	}
}

CartaCfu *giocaInstantPerdita(int numTurno, Player *pPlayer, FILE *fLog) {
	CartaCfu *headCarte        = pPlayer->manoCarteCfu,
	         *choosenCard      = NULL;
	int      i                 = 0,
	         choice            = 0;
	bool     atLeastOneInstant = false,
	         instantCheck      = false;

	while (headCarte != NULL) {
		if (isIstantaneaPerdita(headCarte)) {
			atLeastOneInstant = true;
		}
		printf("[%d] ", i);
		printSingleCartaCfu(headCarte);
		headCarte = headCarte->next;
	}

	if (atLeastOneInstant) {
		do {
			choice    = acquisisciCarta(i);
			headCarte = pPlayer->manoCarteCfu;

			choosenCard = findCartaCfu(&headCarte, choice);
			instantCheck = isIstantaneaPerdita(choosenCard);

			if (!instantCheck) {
				printf("\nHai selezionato una carta che non è istantanea, riprova");
			}

		} while (!instantCheck);

		choosenCard = estraiCartaCfu(&headCarte, choosenCard);
		logPrintLn(fLog, numTurno, pPlayer->username, choosenCard->name);
	} else {
		printf("\nNon hai carte istantanee in mano\n");
		choosenCard = NULL;
	}

	return choosenCard;
}

bool isIstantaneaPerdita(CartaCfu *cartaCfu) {
	bool ret = false;
	if (cartaCfu->effect == SALVA || cartaCfu->effect == DIROTTA) {
		ret = true;
	}
	return ret;
}

void risolviInstantPerdita(Player *playerList, CartaCfu *pCartaCfu,
						   CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo) {
	switch (pCartaCfu->effect) {
		case SALVA:
			effettoSALVA(pCartaCfu, mazzoOstacoli, pCartaOstacolo);
			break;
		case DIROTTA:
			effettoDIROTTA(playerList, pCartaCfu, pCartaOstacolo);
			break;
		default:
			break;
	}
}

void effettoSALVA(CartaCfu *pCartaCfu,
                  CartaOstacolo **mazzoOstacoli, CartaOstacolo **pCartaOstacolo) {
	ostacoloInCoda(*pCartaOstacolo, mazzoOstacoli);
}

void effettoDIROTTA(Player *playerList, CartaCfu *pCartaCfu, CartaOstacolo **pCartaOstacolo) {
	Player *playerHead = NULL;
	playerHead = playerList;
	int count  = 0,
		choice = 0;

	printf("\nA chi vuoi dare questa carta ostacolo?\n");
	while (playerHead != NULL) {
		// Controllo non se stessi
		printf("[%d] %s", count, playerHead->username);
		count++;
		playerHead = playerHead->nextPlayer;
	}
	playerHead = playerList;

	do {
		printf("\n>>> [0 - %d]", count - 1);
		scanf("%d", &choice);

		if (choice < 0 || choice > count - 1) {
			printf("\nInput non valido, riprovare");
		}
	} while (choice < 0 || choice > count - 1);

	for (int i = 0; i < choice; ++i) {
		playerHead = playerHead->nextPlayer;
	}
	ostacoloInCoda(*pCartaOstacolo, &playerHead->listaCarteOstacolo);
}