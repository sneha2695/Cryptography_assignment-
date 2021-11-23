/*Elgamal implemented using gmp */
#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define X "40622201812345"
mpz_t p,q,g;  //Prime numbers 
mpz_t c1, c2; //output values


void generate_prime()
{
	gmp_randstate_t r_state;

	mpz_inits(p, q, NULL);
	gmp_randinit_mt(r_state);

	//generate seed based off of time
	srand(time(0));
	int seed = rand();
	gmp_randseed_ui(r_state, seed);

	//Find random number q
	mpz_urandomb(q, r_state, 512);
	mpz_nextprime(q, q);
	mpz_mul_ui(p, q, 2);
	mpz_add_ui(p, q, 1);

	//Check whether p is prime, if not, keep trying to find prime q and then p
	while (mpz_probab_prime_p(p, 25) != 1) {
		mpz_nextprime(q, q);
		mpz_mul_ui(p, q, 2);
		mpz_add_ui(p, p, 1);
	}

	//gmp_printf("p: %Zd\n", p);
	
	gmp_randclear(r_state);
}

void encryption(mpz_t m)
{
	int seed;
	mpz_t h, r,g,generator,x;
	mpz_t h_pow_r;
	mpz_t p_sub_1_div2, g_x, g_pow_2, one;
	gmp_randstate_t g_state;
	gmp_randstate_t r_state;
	mpz_inits(x,p_sub_1_div2, g_x, g_pow_2, one, NULL);
	mpz_inits(c1, c2,h, r, x,g,generator,h_pow_r, NULL);
	gmp_randinit_mt(g_state);
	gmp_randinit_mt(r_state);
    
	mpz_set_str(x, X, 10);
    gmp_printf("p: %Zd\n", p);
    //find random g
	srand(time(0));
	seed = rand();
	gmp_randseed_ui(g_state, seed);
	mpz_urandomb(g, g_state, 512);
    
	mpz_set_ui(one, 1);
	mpz_set(p_sub_1_div2, p);
	mpz_submul_ui(p_sub_1_div2, one, 0.5);
	//Generator for p 	
	while (mpz_cmp(p,g)<0)
	{
		mpz_powm(g_x, g, p_sub_1_div2, p); //g^((p-1)/2) mod p
		mpz_powm_ui(g_pow_2, g, 2, p); //g^2 mod p

		if (!(mpz_cmp_ui(g_x, 1) == 0) && !(mpz_cmp_ui(g_pow_2, 1) == 0)) {
			mpz_set(generator, g);
			break;
		}
		else{
			srand(time(0));
			int seed = rand();
			gmp_randseed_ui(g_state, seed);
			mpz_urandomb(g, g_state, 512);
		}
	}


	//find random r 
	srand(time(0));
	seed = rand();
	gmp_randseed_ui(r_state, seed);
	mpz_urandomb(r, r_state, 512);
	while( mpz_cmp(p,r)<0) 
	{
	  mpz_urandomb(r, r_state, 512);
    }
 
    gmp_printf("r value : %Zd\n", r);
	gmp_printf("x value : %Zd\n", x); 
	
	//Compute c1 = (g^r)modp
	mpz_powm(c1, g, r, p);

	//Compute h = (g^x)modp
	mpz_powm(h, g, x, p);
	//Compute (h^r)modp
	mpz_powm(h_pow_r, h, r, p);
	
	//Compute c2 = m * h^r
	mpz_mul(c2, m, h_pow_r);

	gmp_printf("Cipher C1 and C2 is as follows : %Zd,%Zd\n\n", c1, c2);
	
	mpz_clear(m);
	mpz_clears(p_sub_1_div2, g_pow_2, g_x, NULL);
	mpz_clears(x, one, g, h_pow_r, h, q, generator, r, NULL);
	gmp_randclear(r_state);
    gmp_randclear(g_state);

}

void decryption()
{
	//Decryption Check
	mpz_t x,c1_powx, c1_powx_inv, m2;
	mpz_inits(x,c1_powx, c1_powx_inv, m2, NULL);

	mpz_set_str(x, X, 10);
	
	//Compute (c1^x)modp
	mpz_powm(c1_powx, c1, x, p);

	//Compute ((c1^x)^-1)modp
	mpz_invert(c1_powx_inv, c1_powx, p);

	//Compute (c2 * (c1^x)^-1)modp
	mpz_mul(m2, c2, c1_powx_inv);
	mpz_mod(m2, m2, p);

	gmp_printf("Decrpyted message: %Zd\n", m2);

	mpz_clears(m2, c1, c2, p, x, c1_powx, c1_powx_inv, NULL);
}

int main()
{
	mpz_t m;

	mpz_init(m);
	
	
	generate_prime();
	
	printf("Enter a value of m: ");
	gmp_scanf("%Zd", m);
	gmp_printf("m: %Zd\n", m);
	
	encryption(m);
	
	decryption();
	
    return 0;
}