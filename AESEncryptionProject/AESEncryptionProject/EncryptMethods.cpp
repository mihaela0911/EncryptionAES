#include "EncryptMethods.h"
#include "CommonMethods.h"
#include "GlobalConstants.h"
#include <fstream>

void encryptPart(char* tempMessage, char* key)
{
	int messageMatrix[MATRIX_SIZE][MATRIX_SIZE];
	int keyMatrix[MATRIX_SIZE][MATRIX_SIZE];

	fillMatrix(messageMatrix, tempMessage);
	fillMatrix(keyMatrix, key);

	int expandedKeys[EXPANDED_KEYS_SIZE];
	keyExpansion(key, expandedKeys); //creating the keys used in all rounds

	executeEncryptRounds(messageMatrix, expandedKeys, keyMatrix);

	char method[] = "Encryted: ";

	writeToFile(messageMatrix, method); //writing the encrypted method in file
}

void addAdditionalLetters(char* message, int index)
{
	while (index % 16 != 0)
	{
		message[index++] = 'm';
	}

	message[index] = '\0';
}

int getLength(char* message)
{
	int index = 0;

	while (message[index])
	{
		index++;
	}

	return index;
}

void readFromFile(char* message, char* key)
{
	std::ifstream myfile;
	myfile.open("inputEncrypt.txt");

	//taking the message and the key needed fot the encryption
	while (myfile.good())
	{
		myfile.getline(message, MAX_SIZE);
		myfile.getline(key, SIZE);
	}

	myfile.close();
}

void executeEncryptRounds(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	int index = KEY_SIZE;

	addRoundKey(messageMatrix, keyMatrix);

	for (size_t i = 1; i < 10; i++)
	{
		takeKey(keyMatrix, expandedKeys, index); //taking the next needed key

		index += KEY_SIZE; //incresing the index to take the next key the folloxilg cicle

		middleRoundsEncrypt(messageMatrix, expandedKeys, keyMatrix);

	}

	takeKey(keyMatrix, expandedKeys, index);

	lastRoundEncrypt(messageMatrix, expandedKeys, keyMatrix);
}

void lastRoundEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	subBytes(messageMatrix);
	shiftRowsLeft(messageMatrix);
	addRoundKey(messageMatrix, keyMatrix);
}

void middleRoundsEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	subBytes(messageMatrix);
	shiftRowsLeft(messageMatrix);
	mixColumns(messageMatrix);
	addRoundKey(messageMatrix, keyMatrix);
}

void shiftRowsLeft(int messageMatrix[][MATRIX_SIZE])
{
	//shifting each row left with the index of the row times

	for (size_t row = 1; row < MATRIX_SIZE; row++)
	{
		for (size_t shif = 0; shif < row; shif++)
		{
			int temp = messageMatrix[row][0];

			for (size_t col = 1; col < MATRIX_SIZE; col++)
			{
				messageMatrix[row][col - 1] = messageMatrix[row][col];
			}

			messageMatrix[row][MATRIX_SIZE - 1] = temp;
		}
	}
}

void mixColumns(int messageMatrix[][MATRIX_SIZE])
{
	int matrixCopy[MATRIX_SIZE][MATRIX_SIZE];
	copyMatrix(messageMatrix, matrixCopy);

	//multiplying the message by the constant matrix to mix the columns
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[0][i] = mul2[matrixCopy[0][i]] ^ mul3[matrixCopy[1][i]] ^ matrixCopy[2][i] ^ matrixCopy[3][i];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[1][i] = matrixCopy[0][i] ^ mul2[matrixCopy[1][i]] ^ mul3[matrixCopy[2][i]] ^ matrixCopy[3][i];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[2][i] = matrixCopy[0][i] ^ matrixCopy[1][i] ^ mul2[matrixCopy[2][i]] ^ mul3[matrixCopy[3][i]];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[3][i] = mul3[matrixCopy[0][i]] ^ matrixCopy[1][i] ^ matrixCopy[2][i] ^ mul2[matrixCopy[3][i]];
	}
}

void subBytes(int messageMatrix[][MATRIX_SIZE])
{
	//substitude the bytes with s-box
	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			int currentByte = messageMatrix[row][col];

			int sBoxRow = currentByte / 16;
			int sBoxCol = currentByte % 16;

			messageMatrix[row][col] = sBox[sBoxRow][sBoxCol];
		}
	}
}