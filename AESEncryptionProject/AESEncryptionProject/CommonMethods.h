#pragma once
#include "GlobalConstants.h"

void writeToFile(int messageMatrix[][MATRIX_SIZE], char* method);
void keyExpansionCore(int* word, int rconIndex);
void keyExpansion(char inputKey[KEY_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE]);
void copyMatrix(int from[][MATRIX_SIZE], int to[][MATRIX_SIZE]);
void addRoundKey(int messageMatrix[][MATRIX_SIZE], int keyMatrix[][MATRIX_SIZE]);
void fillMatrix(int keyMatrix[][MATRIX_SIZE], int* currentKey);
void takeKey(int keyMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int index);
void fillMatrix(int messageMatrix[][MATRIX_SIZE], char* message);

