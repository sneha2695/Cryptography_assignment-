// RSAwithbignumbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "bignum.h"
using namespace std;


int main()
{
	BigNum PArray, QArray, EArray, DArray, NArray, PhiArray, One, msgArray, outArray, msgArray1, outArray1,dP,dQ,m1,m2,qInv;
	string Pstr, Qstr, Estr, Operation, Print,cipher;
	string msg,msg1,str;
	DivResult DR;
	One.Num[0] = 1;
	EArray.Num[0] = 2;
	bool answerp, answerq;
	clock_t t, t1;
	cout << "Enter P ,Q,E values :" << endl;

	cin >> Pstr;
	cin >> Qstr;
	cin >> Estr;

	PArray = StringToArray(Pstr);
	QArray = StringToArray(Qstr);

	answerp = IsPrime(PArray);
	cout << "P is prime :" << answerp<<endl;

	answerq = IsPrime(QArray);
	cout << "Q is prime :" << answerq<<endl;

	if (answerp == false || answerq == false)
	{
		cout << "Not prime values entered :" << endl;
		return 0;
	}


	NArray = Mul(PArray, QArray);
	Print = value_number(NArray);
	cout << Print << endl;

	BigNum PMinusOne = Sub(PArray, One);
	BigNum QMinusOne = Sub(QArray, One);
	PhiArray = Mul(PMinusOne, QMinusOne);

	Print = value_number(PhiArray);
	cout << Print << endl;

	EArray = StringToArray(Estr);
	Print = value_number(EArray);
	cout << "Public key e :" << Print<<endl;

	DArray = Inverse(EArray, PhiArray);
	Print = value_number(DArray);
	cout << "Private key d :" << Print<<endl;

	cout << "Enter value to be encrypted:" << endl;
	cin >> msg;
	msgArray = StringToArray(msg);
	t = clock();
	outArray = PwrMod(msgArray, EArray, NArray);
	t = clock() - t;
	cipher = value_number(outArray);
	cout << "CIpher value:" << cipher << endl;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;
	cout << "Time taken for RSA encryption:" << time_taken << endl;

	msgArray1 = StringToArray(cipher);
	t1 = clock();
	outArray1 = PwrMod(msgArray1, DArray, NArray);
	t1 = clock() - t1;
	msg1= value_number(outArray1);
	cout << "Original message is :" << msg1 << endl;
	double time_taken_decrypt = ((double)t1) / CLOCKS_PER_SEC; // in seconds
	cout << "Time taken for RSA decryption:" << time_taken_decrypt << endl;

	t1 = clock();
	dP = PwrMod(DArray, One, PMinusOne);
	dQ= PwrMod(DArray, One, QMinusOne);
	m1 = PwrMod(msgArray1, dP, PArray);
	m2= PwrMod(msgArray1, dQ, QArray);
	qInv= Inverse(QArray, PArray);
	BigNum m = Sub(m1, m2);
	BigNum q = Mul(qInv, m);
	DR = DivLarge(q, PArray);
	BigNum val = Mul(DR.Remainder, QArray);
	BigNum mes = Add(m2, val);
	t1 = clock() - t1;
	string decrpt = value_number(mes);

	cout << "Decrypted value with CRT:" << decrpt << endl;
	double time_taken_CRT = ((double)t1) / CLOCKS_PER_SEC; // in seconds
	cout << "Time taken for RSA decryption with CRT:" << time_taken_CRT << endl;

	return 0;
}