#include "RLE.h"

RLE GlobalResult;

RLE::RLE(const char* text){
	size = 10;
	dataValue = new char[size + 1];
	count = new unsigned[size + 1];
	char current = text[0];
	bool isDeletable = false;
	int textSize = strlen(text), length = 1, position = 0;
	for (int i = 1; i < textSize + 1; i++){
		if (current != text[i]){
			count [position] = length;
			dataValue[position++] = current;
			if (position >= size - 1)
				resize();
			current = text[i];
			length = 1;
		}
		else{
			length++;
		}
	}
	for (position; position < size; position++){
		dataValue[position] = '\0';
		count[position] = 0;
	}
}

RLE::RLE(const RLE& A){
	dataValue = new char[A.getSize()];
	count = new unsigned[A.getSize()];
	strcpy(dataValue, A.getValues());
	size = A.getSize();
	for (int i = 0; i < size; i++){
		this->count[i] = A.getCounts()[i];
	}
}

RLE::RLE(const char* _dataValue, unsigned* _count, int _size){
	dataValue = new char[_size + 1];
	count = new unsigned[_size + 1];
	size = _size;
	strncpy(dataValue, _dataValue, _size);
	for (int i = 0; i < _size; i++)
		count[i] = _count[i];
	
}

char* RLE::decode() const{
	int newSize = 1, t = 0;
	for (int i = 0; i < size; i++)
		newSize += count[i];
	char* decodedString = new char[newSize];
	for (int i = 0; i < size; i++){
		for (int j = 0; j < count[i]; j++){
			decodedString[t++] = dataValue[i];
		}
	}
	decodedString[newSize - 1] = '\0';
	return decodedString;
}

RLE& RLE::operator=(const RLE& A){
	if (dataValue != NULL)
		delete[] dataValue;
	if (count != NULL)
		delete[] count;
	if (&A != this){
		this->dataValue = new char[A.getSize()];
		this->count = new unsigned[A.getSize()];
		strcpy(this->dataValue, A.getValues());
		this->size = A.getSize();
		for (int i = 0; i < this->size; i++){
			this->count[i] = A.getCounts()[i];
		}
	}
	return *this;
}

ostream& operator<<(ostream& os,const RLE& A){
	if (A.getValues() != NULL && A.getCounts() != NULL)
		for (int i = 0; i < strlen(A.dataValue); i++)
			if (A.getCounts()[i] != 0 && A.getValues()[i] != '\0'){
					os << A.getCounts()[i];
					os << A.getValues()[i];
			}
	return os;
}

const RLE RLE::operator+(const RLE& A) const{
	RLE result(*this);
	result += A;
	return result;
}

RLE& RLE::operator+=(const RLE& A){
	int newSize = getSize() + A.getSize(), j = 0;
	char* tempValues = new char[newSize];
	unsigned* tempCounts = new unsigned[newSize];
	for (int i = 0; i < newSize; i++){
		if (i < getSize() && getValues()[i] != '\0'){
			tempValues[i] = dataValue[i];
			tempCounts[i] = count[i];
		}
		else{
			if (j < A.getSize()){
				if (j == 0 && A.getValues()[j] == tempValues[i - 1])
					tempCounts[i - 1] += A.getCounts()[j++];
				tempValues[i] = A.getValues()[j];
				tempCounts[i] = A.getCounts()[j++];
			}
			else{
				tempValues[i] = '\0';
				tempCounts[i] = 0;
			}
		}
	}
	delete[] count;
	delete[] dataValue;
	dataValue = tempValues;
	count = tempCounts;
	size = newSize;
	return *this;
}

RLE& RLE::operator++(){
	for (int i = size - 1; i >= 0; i--)
		if (count[i] != 0){
			count[i]++;
			return *this;
		}
	return *this;
}

RLE RLE::operator++(int){		
	RLE result(*this);
	operator++();
	return result;
}

RLE& RLE::operator--(){
	for (int i = size - 1; i >= 0; i--)
		if (count[i] != 0){
		--count[i];
		return *this;
		}
	return *this;
}

RLE RLE::operator--(int){
	RLE result(*this);
	operator--();
	return result;
}

// returns a RLE list from position with length
RLE& RLE::operator()(unsigned position, unsigned length){	
	unsigned rlePosition = 0, tempLength = 0;
	int j = 0;
	bool flag = false, done = false;
	unsigned* tempCount = new unsigned[size];
	char* tempDataValue = new char[size];
	for (int i = 0; i < size; i++){
		if (length > tempLength + getCounts()[i] && flag && !done){
			tempLength += getCounts()[i];
			tempCount[j] = getCounts()[i];
			tempDataValue[j++] = getValues()[i];
		}
		else if (flag && !done){
			done = true;
			if (length == tempLength + getCounts()[i]){
				tempCount[j] = getCounts()[i];
				tempDataValue[j++] = getValues()[i];
			}
			else{
				if (tempLength == 0)
					tempCount[j] = length;
				else
					tempCount[j] = tempLength + count[i] - length;
				tempDataValue[j++] = getValues()[i];
			}
		}
		
		if (position > tempLength + getCounts()[i] && !flag){
			rlePosition++;
			tempLength += getCounts()[i];
		}
		else if(!flag){
			flag = true;
			if (position == tempLength + getCounts()[i])
				tempLength = 0;
			else {
				tempCount[j] = tempLength + getCounts()[i] - position;
				tempLength = tempCount[j];
				tempDataValue[j++] = getValues()[i];
			}
		}
	}
	tempCount[j] = 0;
	tempDataValue[j++] = '\0';
	RLE temp(tempDataValue, tempCount, j);
	GlobalResult = temp;
	return GlobalResult;
}

// includes the encoded list from R in *this at position
RLE& RLE::operator()(unsigned position, RLE& R){	
	unsigned tempC = 0;
	bool done = false, done1 = false;
	int sizeA = strlen(dataValue), sizeB = strlen(R.dataValue),
		newSize = sizeA + sizeB + 1, remainder, dataPosition;
	for (int i = 0; i < sizeA && !done; i++){
		if (position > tempC + count[i])
			tempC += count[i];
		else {
			remainder = tempC + count[i] - position;
			dataPosition = i;
			done = true;
			if (remainder == 0)
				newSize--;
		}
	}
	if (R.dataValue[0] == dataValue[dataPosition])
		newSize--;
	if (R.dataValue[sizeB - 1] == dataValue[dataPosition] && sizeB != 1)
		newSize--;
	char* newDataValues = new char[newSize + 1];
	unsigned* newCounts = new unsigned[newSize + 1];
	done = false;
	for (int i = 0; i < newSize; i++){
		if (i < dataPosition){
			newDataValues[i] = dataValue[i];
			newCounts[i] = count[i];
		}
		if (i == dataPosition){
			newDataValues[i] = dataValue[i];
			newCounts[i] = count[i] - remainder;
		}
		if (i > dataPosition && !done){
			newDataValues[i] = R.dataValue[i - dataPosition - 1];
			newCounts[i] = R.count[i - dataPosition - 1];
			if (i == dataPosition + sizeB){
				if (R.dataValue[i - dataPosition - 1] == dataValue[dataPosition]){
					newCounts[i] += remainder;
					dataPosition++;
				}
				done = true;
			}
		}
		else if (done){
			if (i == dataPosition + sizeB + 1 && dataValue[dataPosition] != R.dataValue[sizeB - 1] && !done1){
				newDataValues[i] = dataValue[dataPosition++];
				newCounts[i] = remainder;
				done1 = true;
			}
			else {
				newDataValues[i] = dataValue[dataPosition];
				newCounts[i] = count[dataPosition++];
			}
		}
	}
	newDataValues[newSize] = '\0';
	newCounts[newSize] = 0;
	delete[] count;
	delete[] dataValue;
	dataValue = new char[strlen(newDataValues) + 1];
	count = new unsigned[newSize + 1];
	strcpy(dataValue, newDataValues);
	for (int i = 0; i <= newSize; i++)
		count[i] = newCounts[i];
	delete[] newDataValues;
	delete[] newCounts;
	return *this;
}

bool RLE::operator==(RLE& R){
	bool flag = true;
	for (int i = 0; i < strlen(dataValue); i++){
		if (count[i] != R.count[i] || dataValue[i] != R.dataValue[i])
			flag = false;
	}
	return flag;
}

bool RLE::operator!=(RLE& R){
	return !(*this == R);
}

// returns a RLE list with every unique symbol in *this with its number of repetitions
RLE& RLE::operator*(){						
	char* tempDataValues = new char[size];
	int newSize = 0;
	bool flag = true;
	for (int i = 0; i < size; i++){
		for (int j = newSize; j >= 0; j--){
			if (dataValue[i] == tempDataValues[j])
				flag = false;
			}
			if (flag)
				tempDataValues[newSize++] = dataValue[i];
		flag = true;
	}
	char* finalDataValues = new char[newSize + 1];
	unsigned* tempCounts = new unsigned[newSize + 1];
	for (int i = 0; i < newSize; i++){
		finalDataValues[i] = tempDataValues[i];
		tempCounts[i] = 0;
	}
	delete[] tempDataValues;
	for (int i = 0; i < newSize; i++)
		for (int j = 0; j < size; j++)
			if (finalDataValues[i] == dataValue[j])
				tempCounts[i] += count[j];
			
	
	GlobalResult = RLE(finalDataValues, tempCounts, newSize);
	delete[] finalDataValues;
	delete[] tempCounts;
	return GlobalResult;
}    

// finds the symbol at position n (of the decoded string)
char RLE::operator[](unsigned n){
	unsigned tempLength = 0;
	int position = 0;
	bool flag = true;
	for (int i = 0; i < strlen(dataValue) && flag; i++){
		if (n > tempLength + count[i])
			tempLength += count[i];
		else{
			flag = false;
			position = i;
		}
	}
	return dataValue[position];
}

void RLE::resize(){
	int oldSize = size;
	size += 10;
	char* tempValues = new char[size];
	unsigned* tempCounts = new unsigned[size];
	for (int i = 0; i < size; i++){
		if (i < oldSize){
			tempValues[i] = dataValue[i];
			tempCounts[i] = count[i];
		}
		else{
			tempValues[i] = '\0';
			tempCounts[i] = 0;
		}
	}
	delete[] dataValue;
	delete[] count;
	dataValue = tempValues;
	count = tempCounts;
}

bool RLE::operator<(RLE& R){
	int sizeThis = strlen(dataValue), sizeR = strlen(R.dataValue);
	if (sizeThis > sizeR)
		return false;
	if (*this == R)
		return false;
	bool flag = false;
	for (int i = 0; i < sizeR; i++){
		if (dataValue[0] == R.dataValue[i] && count[0] <= R.count[i]){
			flag = true;
			for (int j = 1; j < sizeThis && j + i < sizeR; j++){
				if ((dataValue[j] == R.dataValue[i + j] && count[j] == R.count[j + i]) || (j == sizeThis - 1 && dataValue[j] == R.dataValue[i + j] && count[j] <= R.count[j + i]))
					flag = true;
				else
					flag = false;
			}
		}
	}
	return flag;
}

bool RLE::operator<=(RLE& R){
	return *this < R || *this == R;
}

bool RLE::operator>(RLE& R){
	return R < *this;
}

bool RLE::operator>=(RLE& R){
	return *this > R || *this == R;
}