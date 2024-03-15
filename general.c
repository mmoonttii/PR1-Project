#include "general.h"

// ============ UTILITIES =================================================
/**
 * randRange() è la subroutine che restiuisce un numero pseudorandom intero compreso tra due interi
 * @param min int: valore inferiore del range
 * @param max int: valore superiore del range
 * @return int: valore pseudorandom generato
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
FILE *openFile(char filePath[], char mode[]) {
	FILE *fp = NULL;

	fp = fopen(filePath, mode);
	// Controllo se l'apertura del file è avvenuta con successo
	if (fp == NULL) exit(ERR_OPEN_FILE);
	return fp;
}

/**
 * enterClear() aspetta fino a due input da parte dell'utente e pulisce lo schermo
 */
void enterClear() {
	printf("Premi INVIO per continuare...");
	// Mi assicuro di pulire il buffer da input preesistenti e aspetto almeno un whitespace dall'utente
	getchar();
	getchar();

	// Pulire lo schermo avviene con chiamate di sistema che sono diverse per ogni sistmea opertavip
	#ifdef _WIN64 // codice per win
		system("cls");
	#elif __APPLE__ // codice per macOs
		system("clear");
	#elif __linux__ // codice per Linux
		system("clear");
	#endif
}

/**
 * minMax() dato un array di interi, ne restituisce il valore minimo e il valore massimo
 * @param arr int[]: punta all'array da elaborare
 * @param size int: num di elementi nell'array
 * @param min int *: puntatore alla locazione di memoria dove verrà salvato il valore minimo
 * @param max int *: punttore alla locazione di memoria dove verrà salvato il valore massimo
 */
void minMax(int arr[], int size, int *min, int *max) {
	// Inizializzo minimo e massimo ai limiti di sistems
	*min = INT_MAX;
	*max = INT_MIN;

	// Ciclo sull'array
	for (int i = 0; i < size; ++i) {
		// Se l'elemento dell'array è minore di quello salvato in min, lo aggiorno
		if (arr[i] < *min) {
			*min = arr[i];
		}

		// Se l'elemento dell'array è minore di quello salvato in min, lo aggiorno
		if (arr[i] > *max) {
			*max = arr[i];
		}
	}
}

/**
 * acquisisciInputInt() attende dall'utente un input intero e si assicura che esso non sia minore di min o maggiore
 * di max
 * @param min int: valore minimo dell'input
 * @param max int: valore massimo dell'input
 * @return int: scelta dell'utente
 */
int acquisisciInputInt(int min, int max) {
	int choice;
	// Aspetto input dell'utente
	do {
		printf("\n>>> ");
		scanf("%d", &choice);

		// Controllo che sia valido
		if (choice < min || choice > max) {
			printf("\nInput non valido, riprovare\n");
		}
	} while (choice < min || choice > max);

	return choice;
}

