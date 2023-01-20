#include "EncryptMethods.h"

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