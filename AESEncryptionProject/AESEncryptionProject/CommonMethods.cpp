#include "CommonMethods.h"
#include <fstream>



void writeToFile(int messageMatrix[][MATRIX_SIZE], char* method)
{
	std::ofstream myfile;
	myfile.open("output.txt", std::fstream::app);
	myfile << method;

	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			myfile << (char)(messageMatrix[col][row]);
		}
	}
	myfile << std::endl;

	if (method[0] == 'E')
	{
		myfile << "ASCII codes: ";
		for (size_t row = 0; row < MATRIX_SIZE; row++)
		{
			for (size_t col = 0; col < MATRIX_SIZE; col++)
			{
				myfile << (messageMatrix[col][row]) << " ";
			}
		}

		myfile << std::endl;

	}

	myfile.close();

}

void keyExpansionCore(int* word, int rconIndex) {
	// Rotate left by one byte: shift left 
	int t = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = t;

	// S-box 4 bytes 

	for (size_t j = 0; j < MATRIX_SIZE; j++)
	{
		int currentByte = word[j];

		int sBoxRow = currentByte / 16;
		int sBoxCol = currentByte % 16;

		word[j] = sBox[sBoxRow][sBoxCol];
	}

	word[0] ^= rcon[rconIndex];
}

void keyExpansion(char inputKey[KEY_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE]) {
	// The first 128 bits are the original key
	for (int i = 0; i < KEY_SIZE; i++) {
		expandedKeys[i] = inputKey[i];
	}

	int bytesGenerated = KEY_SIZE; // Bytes we've generated so far
	int rconIteration = 1; // Keeps track of rcon value
	int tmpCore[MATRIX_SIZE]{ 0 }; // Temp storage for core

	while (bytesGenerated < EXPANDED_KEYS_SIZE) {
		//Read 4 bytes for the core
		for (int i = 0; i < MATRIX_SIZE; i++) {
			tmpCore[i] = expandedKeys[i + bytesGenerated - MATRIX_SIZE];
		}

		// Perform the core once for each 16 byte key
		if (bytesGenerated % KEY_SIZE == 0) {
			keyExpansionCore(tmpCore, rconIteration++);
		}

		for (unsigned char a = 0; a < MATRIX_SIZE; a++) {
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - KEY_SIZE] ^ tmpCore[a];
			bytesGenerated++;
		}

	}
}

void copyMatrix(int from[][MATRIX_SIZE], int to[][MATRIX_SIZE])
{
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		for (size_t j = 0; j < MATRIX_SIZE; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}

void addRoundKey(int messageMatrix[][MATRIX_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	//XOR with the key
	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			messageMatrix[row][col] = (messageMatrix[row][col] ^ keyMatrix[row][col]);
		}
	}
}

void takeKey(int keyMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int index)
{
	int currentKey[KEY_SIZE]{ 0 };
	int keyIndex = 0;

	int lastIndex = index + KEY_SIZE;

	for (int i = index; i < lastIndex; i++)
	{
		currentKey[keyIndex++] = expandedKeys[i];
	}

	fillMatrix(keyMatrix, currentKey); //changin the matrix that keeps the key
}

void fillMatrix(int messageMatrix[][MATRIX_SIZE], char* message)
{
	//takes char array and converts it to 4x4 matrix by columns
	int messageIndex = 0;

	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			messageMatrix[col][row] = message[messageIndex++];
		}
	}
}

void fillMatrix(int keyMatrix[][MATRIX_SIZE], int* currentKey)
{
	int  currentKeyIndex = 0;

	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			keyMatrix[col][row] = currentKey[currentKeyIndex++];
		}
	}
}
