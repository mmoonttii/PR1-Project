#ifndef PR1_PROJECT_GENERAL_H
#define PR1_PROJECT_GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "errors.h"

#define STR_LEN 31
#define LONG_STR 127
#define N_OSTACOLI 4
#define N_PERSONAGGI 4
#define READ "r"

// =========== CARTE CFU =====================================================

/** @brief Enumerazione per i tipi di effetti */
typedef enum {
	NESSUNO, 	/**< Carta senza effetto */
	SCARTAP, 	/**< Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno */
	RUBA, 		/**< Guarda la mano di un collega e ruba una carta a scelta */
	SCAMBIADS, 	/**< Scambia questa carta con quella di un altro giocatore, purché senza effetto */
	SCARTAE, 	/**< Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno */
	SCARTAC, 	/**< Scarta da uno a tre carte dalla tua mano */
	SCAMBIAP, 	/**< Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base */
	DOPPIOE, 	/**< Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti) */
	SBIRCIA, 	/**< Guarda due carte in cima al mazzo, prendine una e scarta l’altra */
	SCAMBIAC,   /**< Scambia le carte punto di due giocatori qualsiasi */
	ANNULLA, 	/**< Annulla gli effetti di tutte le carte punto durante il turno */
	AUMENTA, 	/**< Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta */
	DIMINUISCI, /**< Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta */
	INVERTI, 	/**< Inverti punteggio minimo e massimo del turno */
	SALVA, 		/**< Metti la carta Ostacolo che stai per prendere in fondo al mazzo */
	DIROTTA 	/**< Dai la carta che stai per prendere ad un altro giocatore a tua scelta */
} Effect;


/** @brief Struttura che definisce una carta di tipo CFU */
typedef struct cartaCfu {
	char            name[STR_LEN + 1]; /**< Nome della carta [char]: len 31 + \0 */
	int             cfu; 			   /**< Punti CFU della carta */
	Effect          effect; 		   /**< Effetto della carta */
	struct cartaCfu *next; 			   /**< Puntatore alla prossima carta CFU */
} CartaCfu;


// =========== CARTE OSTACOLO ================================================

/** @brief Enumerazione per i tipi di carte ostacolo*/
typedef enum {
	STUDIO = 1,
	SOPRAVVIVENZA,
	SOCIALE,
	ESAME
} Ostacolo;

/** @brief Struttura che definisce una carta Ostacolo */
typedef struct cartaOstacolo {
	char                 name[STR_LEN + 1];  /**< Nome della carta [char]: len 31 + \0 */
	char                 desc[LONG_STR + 1]; /**< Descrizione della carta [char]: len 127 + \0 */
	Ostacolo             type; 				 /**< Tipologia dell'ostacolo */
	struct cartaOstacolo *next; 			 /**< Puntatore alla prossima carta ostacolo */
} CartaOstacolo;


// =========== GIOCATORE =====================================================

/** @brief Struttura che definisce un personaggio */
typedef struct character {
	char name[STR_LEN + 1]; 	 /**< Nome della carta [char]: len 31 + \0 */
	int  bonusMalus[N_OSTACOLI]; /**< Array per bonus/malus con corrispondenza biunivoca con la tipologia
 									* dell'ostacolo */
} Personaggio;

/** @brief Struttura che definisce il giocatore */
typedef struct player {
	char          username[STR_LEN + 1]; /**< Nome del giocatore [char]: len 31 + \0 */
	Personaggio   character; 			 /**< Personaggio scelto dal giocatore */
	int           cfu; 					 /**< Punti CFU del giocatore */
	CartaCfu      *listaCarte; 			 /**< Lista contenente la mano del giocatore */
	CartaOstacolo *listaOstacoli; 		 /**< Lista degli ostacoli del giocatore */
	struct player *nextPlayer; 			 /**< Giocatore successivo */
} Giocatore;

#endif //PR1_PROJECT_GENERAL_H
