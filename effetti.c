//
// Created by Monti on 26/02/24.
//

#include "effetti.h"

//typedef enum {
//	NESSUNO, 	/**< Carta senza effetto */
//	SCARTAP, 	/**< Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno */
//	RUBA, 		/**< Guarda la mano di un collega e ruba una carta a scelta */
//	SCAMBIADS, 	/**< Scambia questa carta con quella di un altro giocatore, purché senza effetto */
//	SCARTAE, 	/**< Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno */
//	SCARTAC, 	/**< Scarta da uno a tre carte dalla tua mano */
//	SCAMBIAP, 	/**< Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base */
//	DOPPIOE, 	/**< Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti) */
//	SBIRCIA, 	/**< Guarda due carte in cima al mazzo, prendine una e scarta l’altra */
//	SCAMBIAC,   /**< Scambia le carte punto di due giocatori qualsiasi */
//	ANNULLA, 	/**< Annulla gli effetti di tutte le carte punto durante il turno */
//	AUMENTA, 	/**< Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta */
//	DIMINUISCI, /**< Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta */
//	INVERTI, 	/**< Inverti punteggio minimo e massimo del turno */
//	SALVA, 		/**< Metti la carta Ostacolo che stai per prendere in fondo al mazzo */
//	DIROTTA 	/**< Dai la carta che stai per prendere ad un altro giocatore a tua scelta */
//} Effect;


void gestioneEffetti(Turno *turno, Player *playerList, int nPlayers, CartaCfu **mazzoCfu, CartaCfu **mazzoScarti) {
	CartaCfu *headGiocate = turno->carteGiocate,
			 *prev        = NULL;
	Player   *pPlayer     = NULL;
	bool     *isRisolta   = NULL;
	int max = INT_MIN,
		i   = 0;

	isRisolta = allocaArrBool(nPlayers); // isRisolta indica se l'effetto di ogni carta è stato risolto

	for (int j = 0; j < nPlayers; ++j) {
		while (headGiocate != NULL) {
			while (headGiocate->cfu > max && !isRisolta[i]) {
				max = headGiocate->cfu;
			}
			headGiocate = headGiocate->next;
			i++;
		}
		headGiocate = turno->carteGiocate;
		pPlayer = playerList;

		for (int k = 0; k < nPlayers; ++k) {
			if (headGiocate->cfu == max && !isRisolta[k]) {
				if (headGiocate->effect == ANNULLA) {
					printf("Effetto ANNULLA");
					k = nPlayers, j = nPlayers;
				} else {
					isRisolta[k] = risolviEffetti(playerList, headGiocate, k);
				}
			}
		}

	}

	isRisolta = freeBoolArr(isRisolta);
}

bool risolviEffetti(Player *playerList, CartaCfu *cartaCfu, int i) {
	Player *pPlayer = playerList;

	for (int j = 0; j < i; ++j) {
		pPlayer = pPlayer->nextPlayer;
	}

	switch (cartaCfu->effect) {
		case SCARTAP:
//			effettoSCARTAP();
			break;
		case RUBA:
//			effettoRUBA();
			break;
		case SCAMBIADS:
//			effettoSCAMBIADS();
			break;
		case SCARTAE:
//			effettoSCARTAE();
			break;
		case SCARTAC:
//			effettoSCARTAC();
			break;
		case SCAMBIAP:
//			effettoSCAMBIAP();
			break;
		case DOPPIOE:
//			effettoDOPPIOE();
			break;
		case SBIRCIA:
//			effettoSBIRCIA();
			break;
		case SCAMBIAC:
//			effettoSCAMBIAC();
			break;
		case ANNULLA:
//			effettoANNULLA();
			break;
		default:
			printf("");
			break;
	}
	printf("Effetto risolto");
	return true;
}


/**
 * subroutine per l'effetto SCARTAP: \n
 * 	Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno \n
 *
 */
void effettoSCARTAP(CartaCfu **mazzoScarti, Player *pPlayer, Turno *turno, int iPlayer) {
	int nonGiocabile = 0,        // Numero carte non giocabili
		numCarte = 0;            // Numero carte totali
	bool flag = false;           // controllo
	CartaCfu *choosenCard = NULL,   // carta scelta
			 *headMano    = NULL;   // puntatore per scorrere la lista di carte in mano

	headMano = pPlayer->manoCarteCfu;

	if(pPlayer->manoCarteCfu==NULL) { // se la mano è vuota la variabile è settata true
		flag = true;
	}

	while (headMano != NULL) {
		if (isIstantanea(headMano)) { // In questa fase non sono ammesse carte istantanee
			nonGiocabile++;           // incremento le carte non giocabili
		}
		numCarte++; // conto il numero di carte
		headMano = headMano->next; // passaggio alla carta successiva
	}

	headMano = pPlayer->manoCarteCfu;

	if (nonGiocabile != numCarte && flag != false) { // se c'è almeno una carta giocabile
		choosenCard = giocaCarta(&headMano, mazzoScarti, NULL, SPAREGGIO);
		choosenCard = estraiCartaCfu(&headMano, choosenCard);
		turno->points[iPlayer] += choosenCard->cfu;  //assegno i punti della carta al punteggio del giocatore
		printf("%s aggiungi %d CFU al tuo punteggio parziale!\n", pPlayer->username, choosenCard->cfu);
		//la carta viene scartata
		choosenCard->next = (*mazzoScarti);
		(*mazzoScarti) = choosenCard;
	} else { //se non ci sono carte giocabili
		printf("Nessuna carta scartabile in questo turno! Potrebbe essere un bel problema...\n");
	}
}