#pragma once
#include "GlobalConstants.h"

void reverseSubBytes(int messageMatrix[][MATRIX_SIZE]);
void reverseMixColumns(int messageMatrix[][MATRIX_SIZE]);
void shiftRowsRight(int messageMatrix[][MATRIX_SIZE]);
void middleRoundsDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void firstRoundDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void executeDecryptRounds(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void readFromFile(int* message, char* key);

