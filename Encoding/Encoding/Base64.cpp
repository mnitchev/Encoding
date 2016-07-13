#include "Base64.h"

Base64 result_;

//converts a character to a number from 0 to 64;'\0' if character is not recognised
int convertChar(char c){
	if (c >= 'A' && c <= 'Z')
		return c - 65;
	if (c >= 'a' && c <= 'z')
		return c - 97 + 26;
	if (c >= '0' && c <= '9')
		return c - 48 + 52;
	if (c == '+')
		return 63;
	if (c == '/')
		return 64;
	return '\0';
}

//converts a number from 0 to 63 to a character; ' ' if number is not from 0 to 63
char convertInt(unsigned n){
	char result;
	if (n >= 0 && n <= 25)
		result = 65 + n;
	else
		result = ' ';
	if (n >= 26 && n <= 51)
		result = 97 + n - 26;
	if (n >= 52 && n <= 61)
		result = 48 + n - 52;
	if (n == 62)
		result = '+';
	if (n == 63)
		result = '/';
	
	return result;
}

//converts a string to an array of bits
bool* convertStringToBits(unsigned char* text){
	int stringSize = strlen((char*)text) * 8;
	int position = stringSize + (stringSize % 3) * 2;
	unsigned byteSize = 0, charValue;
	bool* stringBits = new bool[position--];
	for (int i = strlen((char*)text) - 1; i >= 0; i--){
		charValue = text[i];
		while (byteSize < 8){
			if (position >= stringSize)
				stringBits[position--] = 0;
			else{
				stringBits[position--] = charValue % 2;
				charValue /= 2;
				byteSize++;
			}
		}
		byteSize = 0;
	}
	return stringBits;
}

//converts an array of bits to string
char* convertBitsToString(bool* stringBits, unsigned size){
	int	position = 0,
		padding = size % 3;
	size += padding * 2;
	int finalSize = size / 6 + padding;
	unsigned charValue = 0;
	char* result = new char[finalSize + 1];
	for (int i = 0; i < size; i++){
 		charValue += stringBits[i] * pow(2, 5 - (i % 6));
		if (i % 6 == 5){
			result[position++] = convertInt(charValue);
			charValue = 0;
		}
	}
	while (position < finalSize)
		result[position++] = '=';
	result[finalSize] = '\0';
	return result;
}

//converts an encoded string to bits
bool* convertEncodedStringToBits(char* _string){
	int padding = 0;
	int stringSize = strlen(_string);
	for (int i = stringSize; i > stringSize - 2; i--)
		if (_string[i] == '=')
			padding++;
	int position = (stringSize - padding) * 6;
	unsigned byteSize = 0, charValue;
	bool* stringBits = new bool[position--];
	for (int i = stringSize - padding - 1; i >= 0; i--){
		charValue = convertChar(_string[i]);
		while (byteSize < 6){
			stringBits[position--] = charValue % 2;
			charValue /= 2;
			byteSize++;
		}
		byteSize = 0;
	}
	return stringBits;
}

Base64::Base64(Base64& B){
	if (B.encodedString != NULL){
		encodedString = new char[strlen(B.encodedString) + 1];
		strcpy(encodedString, B.encodedString);
	}	
	else 
		encodedString = NULL;
}

Base64::Base64(char* text){
	int stringSize = strlen((char*)text) * 8;
	int size = stringSize + (stringSize % 3) * 2;
	bool* stringBits = convertStringToBits((unsigned char*)text);
	char* temp = convertBitsToString(stringBits, stringSize);
	encodedString = new char[strlen(temp) + 1];
	for (int i = 0; i <= strlen(temp); i++)
		encodedString[i] = temp[i];
	delete[] temp;
	delete[] stringBits;
}

Base64& Base64::operator=(Base64& B){
	if (encodedString != NULL)
		delete[] encodedString;
	if (&B != this){
		encodedString = new char[strlen(B.encodedString) + 1];
		strcpy(encodedString, B.encodedString);
	}
	return *this;
}

char* Base64::decode() const{
	int stringSize = strlen(encodedString), padding = 0;
	for (int i = stringSize; i > stringSize - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	int bitSize = (stringSize - padding) * 6 - padding * 2;
	bool* stringBits = convertEncodedStringToBits(encodedString);
	int charValue = 0, position = 0, finalSize = bitSize / 8;
	unsigned char* decodedString = new unsigned char[finalSize + 1];
	for (int i = 0; i < bitSize; i++){
		charValue += stringBits[i] * pow(2, 7 - (i % 8));
		if (i % 8 == 7){
			decodedString[position++] = charValue;
			charValue = 0;
		}
	}
	decodedString[finalSize] = '\0';
	return (char*)decodedString;
}

//returns a symbol wtihin the encoded string on position n
char Base64::operator[](unsigned n){
	int padding = 0;
	for (int i = strlen(encodedString) - 1; i > strlen(encodedString) - 3; i--){
		if (encodedString[i] == '=')
			padding++;
	}
	int sizeBits = strlen(encodedString) * 6 - padding * 2;
	if (n > sizeBits - 8)
		return encodedString[0];
	bool* stringBits = convertEncodedStringToBits(encodedString);
	int position = (n - 1) * 8;
	unsigned char result = 0;
	for (int j = 0; j < 8; j++){
		result += stringBits[position++] * pow(2, 7 - j);
	}
	delete[] stringBits;
	return result;
}

ostream& operator<<(ostream& os, Base64& B){
	return os << B.encodedString;
}

Base64& Base64::operator+(Base64& B){
	bool* stringBitsA = convertEncodedStringToBits(encodedString);
	bool* stringBitsB = convertEncodedStringToBits(B.encodedString);
	int stringSizeA = strlen(encodedString), stringSizeB = strlen(B.encodedString);
	int padding = 0;
	for (int i = stringSizeA; i > stringSizeA - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	int sizeA = (stringSizeA - padding) * 6 - padding * 2;
	padding = 0;
	for (int i = stringSizeB; i > stringSizeB - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	int sizeB = (stringSizeB - padding) * 6 - padding * 2;
	int finalBitSize = sizeA + sizeB + ((sizeA + sizeB) % 3) * 2;
	bool* stringBits = new bool[finalBitSize];
	for (int i = 0; i < finalBitSize; i++){
		if (i >= sizeA + sizeB)
			stringBits[i] = 0;
		else{
			if (i >= sizeA)
				stringBits[i] = stringBitsB[i - sizeA];
			else
				stringBits[i] = stringBitsA[i];
		}
	}
	padding = (sizeA + sizeB) % 3;
	int	position = 0,
		finalSize = finalBitSize / 6 + padding;
	unsigned charValue = 0;
	char* newEncodedString = new char[finalSize + 1];
	for (int i = 0; i < finalBitSize; i++){
		charValue += stringBits[i] * pow(2, 5 - (i % 6));
		if (i % 6 == 5){
			newEncodedString[position++] = convertInt(charValue);
			charValue = 0;
		}
	}
	while (position < finalSize){
		newEncodedString[position++] = '=';
	}
	newEncodedString[finalSize] = '\0';
	result_.setEncodedString(newEncodedString);
	delete[] stringBits;
	delete[] stringBitsA;
	delete[] stringBitsB;
	return result_;
}

Base64& Base64::operator+=(Base64& B){
	return *this = *this + B;
}


void Base64::setEncodedString(char* newEncodedString){
	if (encodedString != NULL)
		delete[] encodedString;
	encodedString = new char[strlen(newEncodedString) + 1];
	strcpy(encodedString, newEncodedString);
}

//returns a substring from the encoded string from startPosition with length number of bytes (8 bits)
Base64& Base64::operator()(unsigned startPosition, unsigned length){
	char* result = new char[length + 1];
	int stringSize = strlen(encodedString), padding = 0;
	for (int i = stringSize; i > stringSize - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	if (startPosition * 8 + length * 8 > stringSize * 6 - padding * 2){
		result_.setEncodedString(" ");
		return result_;
	}
	int bitSize = (stringSize - padding) * 6 - padding * 2, charValue = 0, position = 0;
	bool* stringBits = convertEncodedStringToBits(encodedString);
	for (int i = startPosition*8; i <= startPosition*8 + length*8; i++){
		charValue += stringBits[i] * pow(2, 7 - (i % 8));
		if (i % 8 == 7){
			result[position++] = charValue;
			charValue = 0;
		}
	}
	result[length] = '\0';
	return result_ = Base64((char*)result);
}

//includes the encoded string from B in *this at startPosition
Base64& Base64::operator()(unsigned startPosition, Base64& B){		
	int position = startPosition * 8;
	int stringSizeA = strlen(encodedString), stringSizeB = strlen(B.encodedString);
	unsigned padding = 0;
	for (int i = stringSizeA; i > stringSizeA - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	stringSizeA -= padding;
	unsigned sizeA = stringSizeA * 6 - padding * 2;
	padding = 0;
	for (int i = stringSizeB; i > stringSizeB - 2; i--)
		if (B.encodedString[i] == '=')
			padding++;
	stringSizeB -= padding;
	unsigned sizeB = stringSizeB * 6 - padding * 2;
	bool* stringBitsA = convertEncodedStringToBits(encodedString);
	bool* stringBitsB = convertEncodedStringToBits(B.encodedString);
	bool* tempBits = new bool[sizeA + sizeB];
	bool done = false;
	for (int i = 0; i < sizeA + sizeB ; i++){
		if (!done){
			if (i == position)
				done = true;
			if (i <= position)
				tempBits[i] = stringBitsA[i];
			else
				tempBits[i] = stringBitsA[position++];
		}
		else if (i - position < sizeB){
			tempBits[i] = stringBitsB[i - position];
			if (i - position == sizeB - 1)
				done = false;
		}
	}
	position = 0,
	padding = (sizeA + sizeB) % 3;
	int size = sizeA + sizeB + padding * 2;
	int finalSize = size / 6 + padding;
	unsigned charValue = 0;
	char* result = new char[finalSize + 1];
	for (int i = 0; i < size; i++){
		if (i < sizeA + sizeB)
			charValue += tempBits[i] * pow(2, 5 - (i % 6));
		if (i % 6 == 5){
			result[position++] = convertInt(charValue);
			charValue = 0;
		}
	}
	while (position < finalSize){
		result[position++] = '=';
	}
	result[finalSize] = '\0';
	delete[] encodedString;
	encodedString = new char[strlen(result) + 1];
	strcpy(encodedString, result);
	delete[] result;
	delete[] tempBits;
	delete[] stringBitsA;
	delete[] stringBitsB;
	return *this;
}

bool Base64::operator==(Base64& B){
	if (strcmp(encodedString, B.encodedString) == 0)
		return true;
	else
		return false;
}

bool Base64::operator!=(Base64& B){
	return !(*this == B);
}

bool Base64::operator<(Base64& B){
	if (strlen(B.encodedString) < strlen(encodedString) && *this == B)
		return false;
	if (*this == B)
		return false;
	bool flag = false;
	for (int i = 0; i < strlen(B.encodedString); i++)
		if (encodedString[0] = B.encodedString[i]){
			flag = true;
			for (int j = 1; j < strlen(encodedString) && j + i < strlen(B.encodedString); j++){
				if (encodedString[j] == B.encodedString[j + i])
					flag = true;
				else
					flag = false;
			}
		}
	return flag;
}

bool Base64::operator<=(Base64& B){
	return *this < B || *this == B;
}

bool Base64::operator>(Base64& B){
	return B < *this;
}

bool Base64::operator>=(Base64& B){
	return B < *this || B == *this;
}

//deletes a string of characters from startPosition with length length
void Base64::deleteCharacters(unsigned startPosition, unsigned length){
	char* result = new char[length + 1];
	int stringSize = strlen(encodedString), padding = 0;
	for (int i = stringSize; i > stringSize - 2; i--)
		if (encodedString[i] == '=')
			padding++;
	int bitSize = (stringSize - padding) * 6 - padding * 2, position = 0,
		newBitSize = bitSize - length * 8;
	newBitSize += (newBitSize % 3) * 2;
	if (startPosition * 8 + length * 8 <= bitSize){
		bool* stringBits = convertEncodedStringToBits(encodedString);
		bool* newStringBits = new bool[newBitSize];
		for (int i = 0; i <= bitSize; i++){
			if (i < startPosition * 8 || i > startPosition * 8 + length * 8)
				newStringBits[position++] = stringBits[i];
		}
		while (position < newBitSize)
			newStringBits[position++] = 0;
		char* result = convertBitsToString(stringBits, bitSize - length * 8);
		setEncodedString(result);
		delete[] stringBits;
		delete[] newStringBits;
	}
}