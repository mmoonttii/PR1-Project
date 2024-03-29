#ifndef PR1_PROJECT_STARTING_H
#define PR1_PROJECT_STARTING_H

#include "general.h"
#include "saves.h"

#define NEW_GAME 1
#define LOAD_GAME 2
#define LEAVE 0

void startGame(char *saveName,
			   Character charactersArr[],
			   int *nPlayers, Player **playerList,
			   CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			   CartaOstacolo **mazzoOstacoli);

int mainMenu();

void newGame(char *saveName,
			 Character charactersArr[],
			 int *nPlayers, Player **playerList,
			 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			 CartaOstacolo **mazzoOstacoli);

void initNewGame(Character charactersArr[],
				 int *nPlayers,Player **playerList,
                 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
				 CartaOstacolo **mazzoOstacoli);

void loadGame(char *saveName, int *nPlayers,
			  Player **playerList,
			  CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
              CartaOstacolo **mazzoOstacoli);
#endif //PR1_PROJECT_STARTING_H
