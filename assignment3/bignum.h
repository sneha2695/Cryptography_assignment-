#ifndef _BIGNUM_H
#define _BIGNUM_H
#include<string>
using namespace std;
const int Size2048 = 309;

struct BigNum
{
    int Num[309] = {};
    bool negative = false;
};

struct DivResult
{
    BigNum Result;
    BigNum Remainder;
};

struct ArrayOfArray
{
    BigNum Result;
    BigNum Count;
};

BigNum StringToArray(string input);
BigNum CopyOf(BigNum input);
string value_number(BigNum input);
bool EqualZero(BigNum input);
BigNum Add(BigNum first, BigNum second);
BigNum Sub(BigNum firstOriginal, BigNum second);
BigNum Mul(BigNum first, BigNum second);
BigNum AddFront(BigNum input, int val);
DivResult DivSmall(BigNum first, BigNum second);
DivResult DivLarge(BigNum first, BigNum second);
BigNum PwrMod(BigNum firstOriginal, BigNum secondOriginal, BigNum Mod);
bool IsPrime(BigNum input);
BigNum Inverse(BigNum input, BigNum mod);
bool Equalone(BigNum input);
BigNum gcd(BigNum a, BigNum b);

#endif 