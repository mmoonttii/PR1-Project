//
// Created by Monti on 03/03/24.
//

#ifndef PR1_PROJECT_STARTING_H
#define PR1_PROJECT_STARTING_H

#include "general.h"

#define NEW_GAME 1
#define LOAD_GAME 2
#define LEAVE 0

FILE *startGame(char *saveName, FILE *fSave,
		        Character charactersArr[], int *nPlayers, Player **playerList,
                CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
		        CartaOstacolo **mazzoOstacoli);

FILE *newGame(char *saveName, FILE *fSave,
			  Character charactersArr[], int *nPlayers, Player **playerList,
              CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
			  CartaOstacolo **mazzoOstacoli);

void initNewGame(Character charactersArr[], int *nPlayers,Player **playerList,
                 CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
				 CartaOstacolo **mazzoOstacoli);

FILE *loadGame(char *saveName, FILE *fSave,
               Character charactersArr[], int *nPlayers, Player **playerList,
               CartaCfu **mazzoCfu, CartaCfu **mazzoScarti,
               CartaOstacolo **mazzoOstacoli);
#endif //PR1_PROJECT_STARTING_H
