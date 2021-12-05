#ifndef ELGAMALGMP_H_INCLUDED
#define ELGAMALGMP_H_INCLUDED
#include <gmp.h>

void ElGamalKeyGeneration(mpz_t d,mpz_t beta,mpz_t p,mpz_t alpha);
void ElGamalSign(char *msg,char **e,mpz_t S,mpz_t r,mpz_t p,mpz_t alpha,mpz_t d);
void ElGamalPrestep(mpz_t S,mpz_t t,mpz_t r,mpz_t beta,mpz_t p);
int  ElGamalVer(mpz_t t,char *e,mpz_t alpha,mpz_t p);
#endif