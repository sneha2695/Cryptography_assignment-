#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "Elgamal.h"
#include <string.h>
#include <assert.h>
#include "padding.h"
//#define REPS 10
//#define KLENGTH 30
//#define REPS 40
//#define KLENGTH 100
//#define REPS 120
//#define KLENGTH 300
int main(int argc, char **argv)
{
	mpz_t p, alpha, d, beta, r, S, tp;
	mpz_t res;
	mpz_t *resxx = NULL;
	int REPS = 10, KLENGTH = 30;
	char *e;
	mpz_t t;
	char *message = 0;
	long length;
	mpz_init(res);
	mpz_init(d);
	mpz_init(beta);
	mpz_init(r);
	mpz_init(t);
	mpz_init(S);
	mpz_init(tp);


	message = malloc((100 * sizeof(char)));
	printf("Enter the Message to sign \n");
	scanf("%s",message);
	printf("Message to sign -> %s \n",message);
	printf("_____________________________\n\n");
	groupPrimeOGenerator(p, alpha, KLENGTH, REPS); //   step 1
	gmp_printf("P Large prime  -> %Zd\n", p);
	gmp_printf("Alpha generator of Z*_p -> %Zd\n", alpha);
	printf("_____________________________\n\n");
	ElGamalKeyGeneration(d, beta, p, alpha); // step 2
	gmp_printf("Private key d -> -> %Zd\n", d);
	gmp_printf("Public key Beta -> %Zd\n", beta);
	printf("_____________________________\n\n");
	ElGamalSign(message, &e, S, r, p, alpha, d); //step 3
	printf("Signed S,R:\n");
	mpz_set_str(tp, e, 10);
	gmp_printf("%Zd : ", S);
	gmp_printf("%Zd : ", r);
	ElGamalPrestep(S, t, r, beta, p); //step 4
	printf("\n");
	printf("_____________________________\n\n");
	printf("Elgamal verify:\n");
	printf("\n");
	if (ElGamalVer(t, e, alpha, p))
		printf("Valid Signature\n"); //step 5
	else
		printf("Invalid signature\n");

	mpz_clear(p);
	mpz_clear(alpha);
	return 0;
}