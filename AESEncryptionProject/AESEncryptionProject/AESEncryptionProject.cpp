#include <iostream>
#include <fstream>
#include "DecryptMethods.h"
#include "EncryptMethods.h"
#include "CommonMethods.h"

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

void encrypt()
{
	char message[MAX_SIZE];
	char key[SIZE];
	readFromFile(message, key);

	int messageLength = getLength(message);

	int cycles = messageLength % 16 == 0 ? messageLength / 16 : messageLength / 16 + 1;

	int index = 0;

	for (int i = 0; i < cycles; i++)
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
