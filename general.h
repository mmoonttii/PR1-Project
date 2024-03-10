#ifndef PR1_PROJECT_GENERAL_H
#define PR1_PROJECT_GENERAL_H

// Header di libreria
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// Header user-defined
#include "errors.h"

#define STR_LEN         31     // Lunghezza stringa standard
#define LONG_STR        127    // Lunghezza descrizione carta ostacolo
#define N_OSTACOLI      4      // Numero di tipologie di ostacoli
#define N_PERSONAGGI    4      // Numero di personaggi
#define READ            "r"    // Parametro di lettura files
#define WRITE           "w"    // Parametro di scrittura files
#define CARTE_PER_MANO  5      // Numero di carte che un giocatore deve avere
#define GIOCA_CARTA     1      // Scelta menu: gioca carta
#define INFO_GIOCATORI  2      // Scelta menu: info giocatori
#define LEAVE_GAME      0      // Scelta menu: lascia gioco
#define CFU_WINNER      60     // Numero di CFU per vincere la partita
#define SPAREGGIO       false  // Flag per indicare se gestire il turno come spareggio o come turno regolare
#define SAVES_EXTENSION ".sav"
#define BIN_READ        "rb"
#define BIN_WRITE       "wb"
#define EXTENSION_LEN   4
#define FILE_PERSONAGGI "personaggi.txt"
#define FILE_CARTE_CFU  "carte.txt"
#define FILE_OSTACOLI   "ostacoli.txt"
#define FILE_LOG        "log.txt"
#define DBG             true

// =========== CARTE CFU ===============================================================================================

/** Enumerazione per i tipi di effetti */
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

/** Struttura che definisce una carta di tipo CFU */
typedef struct cartaCfu {
	char            name[STR_LEN + 1]; /**< Nome della carta [char]: len 31 + \0 */
	int             cfu; 			   /**< Punti CFU della carta */
	Effect          effect; 		   /**< Effetto della carta */
	struct cartaCfu *next; 			   /**< Puntatore alla prossima carta CFU */
} CartaCfu;


// =========== CARTE OSTACOLO ==========================================================================================

/** Enumerazione per i tipi di carte ostacolo*/
typedef enum {
	STUDIO = 1,
	SOPRAVVIVENZA,
	SOCIALE,
	ESAME
} Ostacolo;

/** Struttura che definisce una carta Ostacolo */
typedef struct cartaOstacolo {
	char                 name[STR_LEN + 1];  /**< Nome della carta [char]: len 31 + \0 */
	char                 desc[LONG_STR + 1]; /**< Descrizione della carta [char]: len 127 + \0 */
	Ostacolo             type; 				 /**< Tipologia dell'ostacolo */
	struct cartaOstacolo *next; 			 /**< Puntatore alla prossima carta ostacolo */
} CartaOstacolo;


// =========== GIOCATORE ===============================================================================================

/** Struttura che definisce un personaggio */
typedef struct character {
	char name[STR_LEN + 1]; 	 /**< Nome della carta [char]: len 31 + \0 */
	int  bonusMalus[N_OSTACOLI]; /**< Array per bonus/malus con corrispondenza biunivoca con la tipologia
 									* dell'ostacolo */
} Character;

/** Struttura che definisce il giocatore */
typedef struct player {
	char          username[STR_LEN + 1]; /**< Nome del giocatore [char]: len 31 + \0 */
	Character     character; 			 /**< Character scelto dal giocatore */
	int           cfu; 					 /**< Punti CFU del giocatore */
	CartaCfu      *manoCarteCfu; 		 /**< Lista contenente la mano del giocatore */
	CartaOstacolo *listaCarteOstacolo;	 /**< Lista degli ostacoli del giocatore */
	struct player *nextPlayer; 			 /**< Player successivo */
} Player;


// ============ TURNO ==================================================================================================
/** Struttura che definisce un turno di gioco */
typedef struct {
	int numTurno;                   /**< Numero del turno */
	int *points;                    /**< Array dinamico contenente i punteggi del turno di ciascun giocatore, con
 									   * corrispondenza biunivoca tra giocatore e relativo punteggio */
	int cfuToWin;                   /**< Numero di CFU dei giocatori vincenti */
	int cfuToLose;                  /**< Numero di CFU dei giocatori perdenti */
	CartaOstacolo *cartaOstacolo;   /**< Carta ostacolo pescata questo turno */
	CartaCfu *carteGiocate;         /**< Carte giocate dai giocatori a questo turno */
	Player *winners;                /**< Liste di giocatori vincitori */
	Player *losers;                 /**< Lista di giocatori perdenti */
} Turno;

// ============ UTILITIES ==============================================================================================
int randRange(int min, int max);

FILE *openFile(char filePath[], char mode[]);

void enterClear();

void minMax(int arr[], int size, int *min, int *max);
#endif //PR1_PROJECT_GENERAL_H
