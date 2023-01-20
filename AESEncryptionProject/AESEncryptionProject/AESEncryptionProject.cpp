#include <iostream>
#include <fstream>
#include "GlobalConstants.h"

void print(int messageMatrix[][MATRIX_SIZE])
{
	for (size_t row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			std::cout << (char)messageMatrix[col][row];
		}

	}
}

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

void reverseSubBytes(int messageMatrix[][MATRIX_SIZE])
{
	//reversing the substitution of bytes from the s-box
	for (int row = 0; row < MATRIX_SIZE; row++)
	{
		for (size_t col = 0; col < MATRIX_SIZE; col++)
		{
			int currentByte = messageMatrix[row][col];

			int sBoxRow = currentByte / 16;
			int sBoxCol = currentByte % 16;

			messageMatrix[row][col] = reversedSBox[sBoxRow][sBoxCol];
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

void reverseMixColumns(int messageMatrix[][MATRIX_SIZE])
{
	int matrixCopy[MATRIX_SIZE][MATRIX_SIZE];
	copyMatrix(messageMatrix, matrixCopy);

	//multiplying the message by the constant matrix to reverse the mix of columns

	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[0][i] = mul_14[matrixCopy[0][i]] ^ mul_11[matrixCopy[1][i]] ^ mul_13[matrixCopy[2][i]] ^ mul_9[matrixCopy[3][i]];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[1][i] = mul_9[matrixCopy[0][i]] ^ mul_14[matrixCopy[1][i]] ^ mul_11[matrixCopy[2][i]] ^ mul_13[matrixCopy[3][i]];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[2][i] = mul_13[matrixCopy[0][i]] ^ mul_9[matrixCopy[1][i]] ^ mul_14[matrixCopy[2][i]] ^ mul_11[matrixCopy[3][i]];
	}
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		messageMatrix[3][i] = mul_11[matrixCopy[0][i]] ^ mul_13[matrixCopy[1][i]] ^ mul_9[matrixCopy[2][i]] ^ mul_14[matrixCopy[3][i]];
	}
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

void shiftRowsRight(int messageMatrix[][MATRIX_SIZE])
{
	//shifting each row right with the index of the row times
	for (size_t row = 1; row < MATRIX_SIZE; row++)
	{
		for (size_t shift = 0; shift < row; shift++)
		{
			int temp = messageMatrix[row][MATRIX_SIZE - 1];

			for (size_t col = MATRIX_SIZE - 1; col >= 1; col--)
			{
				messageMatrix[row][col] = messageMatrix[row][col - 1];
			}

			messageMatrix[row][0] = temp;
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

void middleRoundsEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	subBytes(messageMatrix);
	shiftRowsLeft(messageMatrix);
	mixColumns(messageMatrix);
	addRoundKey(messageMatrix, keyMatrix);
}

void middleRoundsDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	addRoundKey(messageMatrix, keyMatrix);
	reverseMixColumns(messageMatrix);
	shiftRowsRight(messageMatrix);
	reverseSubBytes(messageMatrix);
}

void lastRoundEncrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	subBytes(messageMatrix);
	shiftRowsLeft(messageMatrix);
	addRoundKey(messageMatrix, keyMatrix);
}

void firstRoundDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	addRoundKey(messageMatrix, keyMatrix);
	shiftRowsRight(messageMatrix);
	reverseSubBytes(messageMatrix);
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

void executeDecryptRounds(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	int index = EXPANDED_KEYS_SIZE - KEY_SIZE;

	firstRoundDecrypt(messageMatrix, expandedKeys, keyMatrix);

	for (size_t i = 1; i < 10; i++)
	{
		index -= KEY_SIZE; //decresing the index to start taking the key from the prevoius line the next cicle

		takeKey(keyMatrix, expandedKeys, index);//taking the next key needed for the next round

		middleRoundsDecrypt(messageMatrix, expandedKeys, keyMatrix);
	}

	index -= KEY_SIZE;
	takeKey(keyMatrix, expandedKeys, index);
	addRoundKey(messageMatrix, keyMatrix);
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

void readFromFile(int* message, char* key)
{
	std::ifstream myfile;
	myfile.open("inputDecrypt.txt");

	//taking the message and the key needed fot the decryption
	for (size_t i = 0; i < SIZE - 1; i++)
	{
		myfile >> message[i];
	}

	myfile.getline(key, SIZE);
	myfile.getline(key, SIZE);

	myfile.close();

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

int getLength(char* message)
{
	int index = 0;

	while (message[index])
	{
		index++;
	}

	return index;
}

void addAdditionalLetters(char* message, int index)
{
	while (index % 16 != 0)
	{
		message[index++] = 'm';
	}

	message[index] = '\0';
}

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

	/*std::cout << "Encrypted message: ";
	print(messageMatrix);*/
	
}

void encrypt()
{
	char message[MAX_SIZE];
	char key[SIZE];
	readFromFile(message, key);

	int messageLength = getLength(message);

	int cycles = messageLength % 16 == 0 ? messageLength / 16 : messageLength / 16 + 1;

	int index = 0;

	for (size_t i = 0; i < cycles; i++)
	{
		char tempMessage[SIZE]{ '\0' };
		int tempMessageIndex = 0;

		for (size_t i = 0; i < 16; i++)
		{
			tempMessage[tempMessageIndex++] = message[index++];

			if (!message[index])
			{
				break;
			}
		}

		tempMessage[tempMessageIndex] = '\0';

		if (getLength(tempMessage) < 16)
		{
			addAdditionalLetters(tempMessage, getLength(tempMessage));
		}

		encryptPart(tempMessage, key);
	}
	std::cout << std::endl;
	std::cout << "Message was encrypted in \'output.txt\'" << std::endl;
}

void decrypt()
{
	int message[SIZE - 1];
	char key[SIZE];
	readFromFile(message, key);

	//std::cout << "Key: " << key << std::endl;

	int messageMatrix[MATRIX_SIZE][MATRIX_SIZE];
	int keyMatrix[MATRIX_SIZE][MATRIX_SIZE];

	fillMatrix(messageMatrix, message);

	int expandedKeys[EXPANDED_KEYS_SIZE];
	keyExpansion(key, expandedKeys); //creating the keys used in all rounds

	int currentKey[KEY_SIZE];
	int lastIndex = EXPANDED_KEYS_SIZE - 16;
	int indexKey = KEY_SIZE - 1;

	for (int i = EXPANDED_KEYS_SIZE - 1; i >= lastIndex; i--)
	{
		currentKey[indexKey--] = expandedKeys[i]; //taking the last key as current
	}

	fillMatrix(keyMatrix, currentKey);

	executeDecryptRounds(messageMatrix, expandedKeys, keyMatrix);

	char method[] = "Decrypted: ";
	writeToFile(messageMatrix, method); //writing the decripted method in file

	std::cout << "Decrypted messege: ";
	print(messageMatrix);
	std::cout << std::endl;

	std::cout << "Message was decrypted in \'output.txt\'" << std::endl;
}

int main()
{
	int command = 0;

	std::cout << "Choose 1 for encrypt or 2 for decrypt: ";
	std::cin >> command;

	if (command == 1)
	{
		encrypt();
	}
	else if (command == 2)
	{
		decrypt();
	}
	else
	{
		std::cout << "Invalid command\n";
	}

}

//  Two One Nine Two
//  Thats my Kung Fu
