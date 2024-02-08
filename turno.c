#include "turno.h"

// ============ TURNO - FASE 1 ================================================

/**
 * Acquisisci azione che si occupa di ricevere l'input del giocatore che corrisponde a un azione da compiere
 * all'inzio del turno
 * @return
 */
int acquisisciAzione() {
	int input;
	printf("\nAzioni:\n"
		   "\t[1] Gioca una carta Cfu\n"
		   "\t[2] Controlla lo stato di altri giocatori (Cfu e carte ostacolo\n"
		   "\t[0] Esci dalla partita\n"
		   ">>> ");
	do {
		scanf("%d", &input);

		if (input > 2 || input < 0){
			printf("Input non valido, riprovare");
		}
	} while (input > 2 || input < 0);
	return input;
}

void giocaCarta(CartaCfu *manoCarteCfu, CartaCfu **listaCarteGiocate) {
	CartaCfu *head = *manoCarteCfu;
	while (head->next != NULL){
		head = head->next;
	}
}

