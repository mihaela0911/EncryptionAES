#pragma once
#include "GlobalConstants.h"

void encryptPart(char* tempMessage, char* key);
void addAdditionalLetters(char* message, int index);
int getLength(char* message);
void readFromFile(char* message, char* key);
void executeEncryptRounds(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void lastRoundEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void middleRoundsEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE]);
void shiftRowsLeft(int messageMatrix[][MATRIX_SIZE]);
void mixColumns(int messageMatrix[][MATRIX_SIZE]);
void subBytes(int messageMatrix[][MATRIX_SIZE]);
