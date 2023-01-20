#pragma once
void reverseSubBytes(int messageMatrix[][MATRIX_SIZE])
{
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
void reverseMixColumns(int messageMatrix[][MATRIX_SIZE])
{
	int matrixCopy[MATRIX_SIZE][MATRIX_SIZE];
	copyMatrix(messageMatrix, matrixCopy);

	//print(matrixCopy);

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
void shiftRowsRight(int messageMatrix[][MATRIX_SIZE])
{
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
void middleRoundsDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	addRoundKey(messageMatrix, keyMatrix);
	reverseMixColumns(messageMatrix);
	shiftRowsRight(messageMatrix);
	reverseSubBytes(messageMatrix);
}

void firstRoundDecrypt(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	addRoundKey(messageMatrix, keyMatrix);
	shiftRowsRight(messageMatrix);
	reverseSubBytes(messageMatrix);
}

void executeDecryptRounds(int messageMatrix[][MATRIX_SIZE], int expandedKeys[EXPANDED_KEYS_SIZE], int keyMatrix[][MATRIX_SIZE])
{
	int index = EXPANDED_KEYS_SIZE - KEY_SIZE;

	firstRoundDecrypt(messageMatrix, expandedKeys, keyMatrix);


	for (size_t i = 1; i < 10; i++)
	{
		index -= KEY_SIZE;

		takeKey(keyMatrix, expandedKeys, index);

		middleRoundsDecrypt(messageMatrix, expandedKeys, keyMatrix);
	}

	index -= KEY_SIZE;

	takeKey(keyMatrix, expandedKeys, index);
	addRoundKey(messageMatrix, keyMatrix);
}
void readFromFile(int* message, char* key)
{
	std::ifstream myfile;
	myfile.open("inputDecrypt.txt");

	for (size_t i = 0; i < SIZE - 1; i++)
	{
		myfile >> message[i];
	}

	myfile.getline(key, SIZE);
	myfile.getline(key, SIZE);

	myfile.close();

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
	keyExpansion(key, expandedKeys);

	int currentKey[KEY_SIZE];
	int lastIndex = EXPANDED_KEYS_SIZE - 16;
	int indexKey = KEY_SIZE - 1;

	for (int i = EXPANDED_KEYS_SIZE - 1; i >= lastIndex; i--)
	{
		currentKey[indexKey--] = expandedKeys[i];
	}

	fillMatrix(keyMatrix, currentKey);

	executeDecryptRounds(messageMatrix, expandedKeys, keyMatrix);

	char method[] = "Decrypted: ";
	writeToFile(messageMatrix, method);

	std::cout << "Decrypted messege: ";
	print(messageMatrix);
	std::cout << std::endl;

	std::cout << "Message was decrypted in \'output.txt\'" << std::endl;
}
