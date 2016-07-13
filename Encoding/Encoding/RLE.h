#ifndef RLE_H_
#define RLE_H_

#include <iostream>
#include <cstring>
using namespace std;

class RLE{
	char* dataValue;
	unsigned* count;
	int size; 
public:
	RLE(){ dataValue = NULL; count = NULL; size = 0; }
	RLE(const char*);
	RLE(const RLE&);
	RLE(const char*, unsigned*, int);
	~RLE(){ delete[] dataValue; delete[] count; }
	char* decode() const;
	RLE& operator=(const RLE&);
	friend ostream& operator<<(ostream&,const RLE&);
	const RLE operator+(const RLE&) const;	
	RLE& operator+=(const RLE&);
	RLE& operator++();
	RLE operator++(int);
	RLE& operator--();
	RLE operator--(int);
	RLE& operator()(unsigned, unsigned);
	RLE& operator()(unsigned, RLE&);
	bool operator==(RLE&);
	bool operator!=(RLE&);
	RLE& operator*();
	char operator[](unsigned);
	bool operator<(RLE&);
	bool operator>(RLE&);
	bool operator<=(RLE&);
	bool operator>=(RLE&);

	void resize();

	const char* getValues() const { return dataValue; };
	const unsigned* getCounts() const { return count; }
	int getSize() const { return size; }

};

#endif