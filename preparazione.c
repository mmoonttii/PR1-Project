//
// Created by Monti on 26/01/24.
//

#include "preparazione.h"

// ============ UTILITIES =================================================
/**
 * randRange() è la funzione che, restiuisce un numero pseudorandom intero compreso tra due interi
 * @param min valore inferiore del range
 * @param max valore superiore del range
 * @return intero pseudorandom generato
 */
int randRange(int min, int max) {
	int random;
	random = min + rand() % (max - min + 1);
	return random;
}

/**
 * openFile() è la funzione che apre i file forniti in modalità lettura
 *
 * @param filePath è il percorso al file da aprire
 * @param mode è la modalità di apertura
 * @return è un puntatore al file aperto
 */
FILE *openFile(char filePath[], char mode[]){
	FILE *fp = NULL;
	fp = fopen(filePath, mode);
	if (fp == NULL) exit(ERR_OPEN_FILE);
	return fp;
}


// ============ CARTE CFU =====================================================
/**
 * creaMazzoCfu() è la subroutine che legge il file delle carte CFU e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [carte.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaCfu *creaMazzoCfu(FILE *fp) {
	int read       = 0,       // Numero di letture avvenute con successo
		occurences = 0, // Numero di carte dello stesso tipo
		carte      = 0,      // Numero di carte nel mazzo
		pos        = 0;         // Posizione in cui inserire una nuova carta
	CartaCfu cartaCfu,        // Struttura ausiliaria dove viene salvata la nuova carta letta
			 *newCard = NULL, // Puntatore per l'allocazione di una nuova carta
			 *mazzo   = NULL,   // Puntatore all'inizio del mazzo [return]
			 *auxP    = NULL,    // Puntatore ausiliario
			 *head    = NULL;   // Puntatore alla testa

	do {
		// Lettura riga dal file, se read == 4 lettura success
		read = 0;
		read = fscanf(fp, "%d %d %d %[^\n]31s", &occurences, &cartaCfu.effect, &cartaCfu.cfu, cartaCfu.name);
		fscanf(fp, "\n");

		cartaCfu.next = NULL;

		// Se lettura success
		if (read == 4) {
			// Per ogni carte di tipo x
			while (occurences > 0) {
				newCard = allocaCartaCfu(); // Alloca nuova carta e salva in var
				*newCard = cartaCfu; // Inizializza spazio allocato con dati letti

				if (mazzo == NULL) {
					// Se il mazzo è vuoto la carta letta diventa la testa del mazzo
					mazzo = newCard;
				} else {
					// Altrimenti genera posizione in cui aggiungere la carta
					// -1 aggiungere in testa
					// n aggiungere dopo carta di indice n
					// carte - 1 aggiungere in coda
					pos = randRange(-1, carte - 1);

					// Aggiunta in testa
					if (pos == -1) {
						auxP  = mazzo; // Salvo testa del mazzo precedente
						mazzo = newCard; // La nuova testa del mazzo diventa la carta appena allocata
						mazzo->next = auxP; // Dopo la nuova testa collego il resto del mazzo vecchio

					// Aggiunta in coda
					} else if (pos == carte - 1){
						// Raggiungo l'ultima posizione della lista
						while (head != NULL){
							// Salvo il puntatore alla struttura attuale
							auxP = head;
							head = head->next;
							// Quando head è NULL, sono andato troppo avanti
						}
						// Quinid ritorno alla posizione precedente
						head = auxP;
						// Il campo next dell'attuale head, ultimo della lista, può accogliere la carta appena allocata
						head->next = newCard;
						newCard->next = NULL;
					} else {
						// Scorro fino alla carta in posizione pos
						for (int i = 0; i < pos; ++i) {
							head = head->next;
						}
						auxP = head->next; // Salvo il puntatore alla carta successiva a pos
						head->next    = newCard; // La successiva alla carta pos, sarà la nuova carta
						newCard->next = auxP; // Dopo la nuova carta riaggiungo il resto del mazzo
					}
				}
				head = mazzo;
				occurences--; // Il numero di carte dello stesso tipo ora lette è diminuito
				carte++; // Il numero delle carte totali nel mazzo è aumentato
			}
		} else {
			// In caso di lettura fallita
			printf("\n--printf scurrile rimosso--\n");
		}
	} while (read == 4);
	printf("\nCarte lette %d\n", carte);

	return mazzo;
}

/**
 * distribuisciCarte è la subroutine che si occupa di distribuire carte ai giocatori fin quando questi non hanno
 * 5 carte in mano
 * @param mazzoCfu è un puntatore al puntatore alla prima carta del mazzo
 * @return un puntatore alla prima carta del mazzo
 */
CartaCfu *distribuisciCarte(CartaCfu *mano, CartaCfu **mazzoCfu){
	CartaCfu *head = mano;
	int counter = 0;

	// Se la mano non è vuota, scorro fino all'ultima carta, tenendo conto di quante carte sto scorrendo
	while (head != NULL && head->next != NULL){
		head = head->next;
		counter++;
	}
	// Fin quando la mano è composta da 5 carte o meno
	while (counter < 5){
		// Se il mazzo delle carte è vuoto
		if (mazzoCfu == NULL){
			// Rimescolo il mazzo degli scarti
			// TODO: mescola mazzo
			// mazzoCfu = mescolaMazzo(mazzoScarti);
		}
		// Se la testa è NULL, questo si verifica nel caso di una mano completamente vuota
		if (head == NULL){
			// La testa della mano prende la testa del mazzo
			head = *mazzoCfu;
			// La testa del mazzo diventa la carta successiva a quella appena "distribuita"
			*mazzoCfu = (*mazzoCfu)->next;
			// Salvo la nuova testa come inizio della mano
			mano = head;

		// Se la testa non è null, quindi abbiamo già almeno una carta in mano
		} else {
			// La prossima carta della mano diventa la prima carta del mazzo
			head->next = *mazzoCfu;
			// La prima carta del mazzo diventa la carta successiva a quella appena "distribuita"
			*mazzoCfu = (*mazzoCfu)->next;
			// La testa diventa la carta successiva
			head = head->next;
		}
		// Incremento il contatore che tiene traccia delle carte nella mano
		counter++;
	}
	head->next = NULL;

	return mano;
}

/**
 * printCarteCfu è la subroutine che si occupa di stampare una lista di carte
 * @param listaCarteCfu è la lista da stampare
 */
void printCarteCfu(CartaCfu *listaCarteCfu) {
	CartaCfu *head = listaCarteCfu; // Testa della lista
	int count = 0; // Contatore delle carte
	bool stop = true; // Condizione di uscita dalla stampa
	char *effetti[] = {"Carta senza effetto",
					   "Scarta una carta CFU punto e aggiungi il suo punteggio quello del turno",
					   "Guarda la mano di un collega e ruba una carta a scelta",
					   "Scambia questa carta con quella di un altro giocatore, purché senza effetto",
					   "Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno",
					   "Scarta da uno a tre carte dalla tua mano",
					   "Scambia il punteggio del turno maggiore e minore dopo il calcolo del punteggio di base",
					   "Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti)",
					   "Guarda due carte in cima al mazzo, prendine una e scarta l’altra",
					   "Scambia le carte punto di due giocatori qualsiasi",
					   "Annulla gli effetti di tutte le carte punto durante il turno",
					   "Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta",
					   "Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta",
					   "Inverti punteggio minimo e massimo del turno",
					   "Metti la carta Ostacolo che stai per prendere in fondo al mazzo",
					   "Dai la carta che stai per prendere ad un altro giocatore a tua scelta"
	};

	printf("\n==== CARTE CFU ====\n");
	// Finchè la condizione di stop è rispettata
	while (stop){
		// Stampa la struttura carta
		printf("Nome: %s,\n"
			   "CFU: %d,\t Effetto: %s\n",
			   head->name, head->cfu, effetti[head->effect]);
		// In caso la prossima carta sia NULL, esci dal loop
		if (head->next == NULL){
			stop = false;
		// Altrimenti continua con la prossima carta
		} else {
			head = head->next;
		}
		count++;
	}
	printf("\nLe carte sono %d\n", count);

}
// ============ CARTE OSTACOLO ================================================

/**
 * creaMazzoOstacoli() è la subroutine che legge il file delle carte Ostacolo e crea un mazzo mischiato delle carte
 *
 * @param fp è il file da leggere [ostacoli.txt]
 * @return un puntatore alla testa del mazzo
 */
CartaOstacolo *creaMazzoOstacoli(FILE *fp) {
	int      nOstacoliTipo,
	         carte = 0,
	         pos;
	Ostacolo tipo = 1;
	CartaOstacolo cartaOstacolo,
				  *mazzo = NULL,
				  *newCard = NULL,
				  *head = NULL,
				  *auxP = NULL;

	for (int i = 0; i < N_OSTACOLI; ++i) {
		fscanf(fp, "%d\n", &nOstacoliTipo);

		for (int j = 0; j < nOstacoliTipo; ++j) {

			fscanf(fp, "%31s", cartaOstacolo.name);
			fscanf(fp, "\n");
			fscanf(fp, "%127s", cartaOstacolo.desc);
			fscanf(fp, "\n");
			cartaOstacolo.type = tipo;

			newCard = allocaCartaOstacolo();
			*newCard = cartaOstacolo;

			if (mazzo == NULL){
				mazzo = newCard;
				mazzo->next = NULL;
			} else {
				// pos è la posizione nel quale inserire una nuova carta, dove
				// 0 è la posizione immediatamente successiva alla prima carta del mazzo
				// -1 è la posizione come nuova prima carta del mazzo
				pos = randRange(-1, carte - 1);

				// Posizionamento carta all'inizio del mazzo
				if (pos == -1) {
					auxP = mazzo;
					mazzo = newCard;
					mazzo->next = auxP;
				} else {
					for (int k = 0; head != NULL && k < pos; ++k) {
						head = head->next;
					}
					if (head != NULL) {
						auxP = head->next; // Salvo il puntatore alla carta che risulterà la successiva a quella letta
						head->next = newCard; // La nuova carta viene inserita in posizione successiva a head
						newCard->next = auxP; // Il resto della lista viene messo successivamente alla nuova carta
					} else {
						head = newCard;
						head->next = NULL;
					}
				}
			}
			head = mazzo;
			carte++;
		}
		tipo++;
	}
	return mazzo;
}


// ============ PERSONAGGI E GIOCATORI ========================================
/**
 * parseCharacters() è la funzione che legge il file dei personaggi e conserva le informazioni di ogni personaggio in
 * un'array di sturtture personaggio
 *
 * Una struttura characters nel campo bonusMalus abbiamo un array dove vengono salvati gli eventuali bonus e malus di
 * ogni giocatore per ogni tipologia di ostacolo
 *
 * @param fp è il file da leggere [personaggi.txt]
 * @param characters è l'indirizzo dell'array di strutture personaggio
*/
void parseCharacters(FILE *fp, Personaggio characters[]) {
	// Per ogni personaggio
	for (int i = 0; i < N_PERSONAGGI; i++){
		// Per ogni ostacolo
		for (int j = 0; j < N_OSTACOLI; j++){
			fscanf(fp, "%d ", &characters[i].bonusMalus[j]);
		}
		fscanf(fp, "%[^\n]31s", characters[i].name);
		fscanf(fp, "\n");
	}
}


/**
 * acquisisciNumGiocatore() è la funzione che acquisisce il numero dei partecipanti alla partita
 * La funzione si occupa di controllare che l'input sia valido e invita a ritentare in caso di input non valido
 * @return il numero di giocatori alla partita
 */
int acquisisciNumGiocatori() {
	int nGiocatori,
		read = 0;

	do {
		printf("Quanti giocatori parteciperanno oggi? [2-4] ");
		read = scanf(" %d", &nGiocatori);

		if (nGiocatori < 2 || nGiocatori > 4){
			printf("Il numero di giocatori deve essere compreso tra 2 e 4:\n"
				   "\t controlla l'input o organizzatevi in squadre :)\n");
		}
	} while (nGiocatori < 2 || nGiocatori > 4);

	return nGiocatori;
}


/**
 * initGiocatori() è la funzione che, dati il numero dei giocatori e il mazzo di carte Cfu, restituisce una lista di
 * giocatori, occupandosi di inizializzarla, assegnando un username, personaggio e una mano di cinque carte iniziali
 * @param nGiocatori è il numero di giocatori che partecipano alla partita
 * @param mazzoCfu è un puntatore alla testa della lista che rappresenta il mazzo delle carte Cfu, passata alla
 * funzione per permettere l'assegnamento della mano iniziale di carte
 * @return la funzione restituisce listaGiocatori, una lista di strutture di tipo giocatore
 */
Giocatore *initGiocatori(int nGiocatori, CartaCfu **mazzoCfu) {
	Giocatore *listaGiocatori = NULL,
			  *head           = NULL,
			  *newPlayer      = NULL,
			  aux;

	printf("\n=== PARTECIPANTI ===\n");
	// Per ogni giocatore
	for (int i = 0; i < nGiocatori; ++i) {
		// Richiedi username
		printf("\nGIOCATORE %d\n", i);
		printf("Inserire username: ");
		scanf(" %[^\n]31s", aux.username);

		// Inizializza la struttura
		aux.cfu = 0; // Punteggio di partenza
		aux.listaCarte = NULL;
		aux.listaCarte = distribuisciCarte(aux.listaCarte, mazzoCfu); // Mano iniziale
		aux.listaOstacoli = NULL;
		aux.nextPlayer = NULL;

		// Creazione lista dei giocatori
		if (listaGiocatori == NULL) {
			listaGiocatori = allocaGiocatore();
			*listaGiocatori = aux;
			head = listaGiocatori;
		} else {
			head->nextPlayer = allocaGiocatore();
			*head->nextPlayer = aux;
			head = head->nextPlayer;
		}

	}
	return listaGiocatori;
}

/**
 * printGiocatori() è la subroutine che stampa la lista dei giocatori, il loro nome, personaggio, punteggio e mano
 * @param listaGiocatori è la lista di giocatori da stampare
 */
void printGiocatori(Giocatore *listaGiocatori) {
	Giocatore *head = listaGiocatori;
	printf("\n=== GIOCATORI ===\n");

	for (int i = 0; head != NULL; ++i) {
		printf("\n+----------------------------------------\n"
			   "|Giocatore %d\n"
			   "|\t Username: %s\n"
			   "|\t Cfu: %d\n",
			   i, head->username, head->cfu);
		printCarteCfu(head->listaCarte);
		printf("+----------------------------------------\n");
		head = head->nextPlayer;
	}
}
