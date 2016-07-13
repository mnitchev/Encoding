#ifndef BASE64_H_
#define BASE64_H_

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

class Base64{
	char* encodedString;
public:
	Base64() { encodedString = NULL; }
	Base64(Base64&);
	Base64(char*);
	Base64& operator=(Base64&);
	~Base64() { delete[] encodedString; }

	char* decode() const;
	char operator[](unsigned);
	friend ostream& operator<<(ostream&, Base64&);
	Base64& operator+(Base64&);
	Base64& operator+=(Base64&);
	Base64& operator()(unsigned, unsigned);
	Base64& operator()(unsigned, Base64&);
	bool operator<=(Base64&);
	bool operator<(Base64&);
	bool operator>=(Base64&);
	bool operator>(Base64&);
	bool operator==(Base64&);
	bool operator!=(Base64&);
	void deleteCharacters(unsigned, unsigned); 

	void setEncodedString(char* newEncodedString);
	friend bool* convertEncodedStringToBits(char*);

};

bool* convertStringToBits(unsigned char*);
char* convertBitsToString(bool*, unsigned);
int convertChar(char);

#endif
