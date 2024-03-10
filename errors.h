
#ifndef PR1_PROJECT_ERRORS_H
#define PR1_PROJECT_ERRORS_H

// Errore apertura file
#define ERR_OPEN_FILE    101

// ====================================

// Errori allocazione          [2xx]
#define ERR_FAIL_ALLOC_CFU      202
#define ERR_FAIL_ALLOC_OSTACOLO 203
#define ERR_FAIL_ALLOC_PLAYER   204
#define ERR_FAIL_ALLOC_POINTS   205
#define ERR_FAIL_ALLOC_BOOLS    206

// Errore menu
#define ERR_MENU      301
#define ERR_PLAY_CARD 302

// Errori lettura file                [4xx]
#define ERR_READ_SAVE_N_PLAYERS        401
#define ERR_READ_SAVE_PLAYER           402
#define ERR_READ_SAVE_MANO_CFU         403
#define ERR_READ_SAVE_N_OSTACOLI       404
#define ERR_READ_SAVE_OSTACOLI         405
#define ERR_READ_SAVE_N_MAZZO_CFU      406
#define ERR_READ_SAVE_MAZZO_CFU        407
#define ERR_READ_SAVE_N_MAZZO_SCARTI   408
#define ERR_READ_SAVE_MAZZO_SCARTI     409
#define ERR_READ_SAVE_N_MAZZO_OSTACOLI 410
#define ERR_READ_SAVE_MAZZO_OSTACOLI   411

#endif //PR1_PROJECT_ERRORS_H

