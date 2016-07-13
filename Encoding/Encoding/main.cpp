#include "RLE.h"
#include "Base64.h"

int main(){
	char imput[1000];
	cout << "Imput Base64 object A: ";
	cin.getline(imput, 1000);
	Base64 A(imput);
	cout << endl;
	cout << "Imput Base64 object B: ";
	cin.getline(imput, 1000);
	Base64 B(imput);
	cout << endl;
	cout << "Imput RLE object C: ";
	cin.getline(imput, 1000);
	RLE C(imput);
	cout << endl;
	cout << "Imput RLE object D: ";
	cin.getline(imput, 1000);
	RLE D(imput);
	cout << endl;
	cout << "Object A: " << A << endl;
	cout << "Object B: " << B << endl;
	cout << "Object C: " << C << endl;
	cout << "Object D: " << D << endl;
	cin.get();
	int n;
	cout << "Input a number : "; cin >> n;
	cout << "A[n] = " << A[n] << endl;
	cout << "B[n] = " << B[n] << endl;
	cout << "C[n] = " << C[n] << endl;
	cout << "D[n] = " << D[n] << endl;
	cin.get();
	cout << "A + B:  " << (A + B) << endl;
	cout << "A += B: " << (A += B) << endl;
	cout << "C + D   " << (C + D) << endl;
	cout << "C += D  " << (C += D) << endl;
	cin.get();
	cout << "C++ = " << C++ << endl;
	cout << "++D = " << ++D << endl;
	cout << "C-- = " << C-- << endl;
	cout << "--D = " << --D << endl;
	cin.get();
	cout << "*C = " << *C << endl;
	cout << "*D = " << *D << endl;
	cin.get();
	cout << "A > B  = " << (A > B) << endl;
	cout << "C < D  = " << (C < D) << endl;
	cout << "A == B = " << (A == B) << endl;
	cout << "C == D = " << (C == D) << endl;

	cout << endl << "decode :" << A.decode() << " " << C.decode() << endl;
	cout << A(n, B) << " " << A.decode() << endl << A(n, n) << endl << C(n, D) << endl << C(n, n);
	cin.get();
	return 0;
}