#include "general.h"


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
FILE *openFile(char filePath[], char mode[]) {
	FILE *fp = NULL;
	fp = fopen(filePath, mode);
	if (fp == NULL) exit(ERR_OPEN_FILE);
	return fp;
}

/**
 * enterClear() è la subroutine che aspetta dell'input da parte di un giocatore e pulisce lo schermo
 */
void enterClear() {
	printf("Premi INVIO per continuare...");
	getchar();
	getchar();

	#ifdef _WIN64 // codice per win
		system("cls");
	#elif __APPLE__ // codice per macOs
		system("clear");
	#elif __linux__ // codice per Linux
		sys	prtem("clear");
	#endif
}

/**
 * minMax() è una subroutine che, dato un array di interi, ne restituisce il valore minimo e il valore massimo
 * @param arr è un puntatore all'array da elaborare
 * @param size è il numero di elementi nell'array
 * @param min è un puntatore alla locazione di memoria dove verrà salvato il valore minimo
 * @param max è un punttore alla locazione di memoria dove verrà salvato il valore massimo
 */
void minMax(int arr[], int size, int *min, int *max) {
	*min = INT_MAX,
	*max = INT_MIN;

	for (int i = 0; i < size; ++i) {
		if (arr[i] < *min) {
			*min = arr[i];
		}
		if (arr[i] > *max) {
			*max = arr[i];
		}
	}
}
